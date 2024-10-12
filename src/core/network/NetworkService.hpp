#ifndef NETWORKSERVICE_HPP
    #define NETWORKSERVICE_HPP

/**
 * @file NetworkingService.hpp
 * @brief Defines the NetworkingService class, which provides UDP-based networking functionality.
 */
    #include <asio.hpp>
    #include <chrono>
    #include <cstdint> // Pour les types uint8_t, uint16_t, uint64_t
    #include <cstring>
    #include <iostream>
    #include <string>
    #include <thread>
    #include <vector>
    #include <map>
    #include "./includes/RequestHeader.hpp"

/**
 * @class NetworkingService
 * @brief Provides networking services for sending and receiving UDP packets, handling various types of GDTP messages.
 *
 * The NetworkingService class handles UDP-based client-server communications, including sending requests, receiving packets,
 * and processing messages based on GDTP message types. It uses the ASIO library for asynchronous network I/O. The class follows
 * the singleton design pattern to ensure that only one instance exists across the entire application.
 */
class NetworkingService {
public:
    /**
     * @brief Returns the singleton instance of NetworkingService.
     *
     * This method ensures that there is only one instance of NetworkingService throughout the program.
     * If the instance is not yet created, it initializes it with the provided IP address and port.
     * If it has already been created, it returns the same instance.
     *
     * @param port The port number for the server (default is 12345).
     * @return A reference to the singleton instance of NetworkingService.
     *
     * @note The  port are used only during the first call to this method. Subsequent calls
     * will return the existing instance without reinitializing the networking parameters.
     *
     * @warning This method ensures that only one instance of NetworkingService exists. Do not attempt to
     * create an instance manually outside this method, as it may lead to undefined behavior.
     *
     * @code
     * // Example usage:
     * NetworkingService& networkService = NetworkingService::getInstance("192.168.1.1", 8080);
     * networkService.sendRequest("127.0.0.1", 12345, uint8_t::PlayerMovement, {"1", "100.0", "200.0", "300.0"});
     * @endcode
     */
    static NetworkingService& getInstance(
        const int port = 0
    ) {
        static NetworkingService instance(port);
        return instance;
    }

    NetworkingService(const NetworkingService&) = delete;
    NetworkingService& operator=(const NetworkingService&) = delete;

    /**
    * @brief Constructs a NetworkingService object and starts receiving packets on the specified IP and port.
    * @param port The port number to listen on.
    */
    NetworkingService(
        const int port
    ) : _port(port){
        message_handlers = std::make_shared<std::map<uint8_t, std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>>>();
    }

    /**
     * @brief Sets the handler function for a specific GDTP message type.
     *
     * This method allows the user to assign a handler function that will be called when a message
     * of a specific GDTP message type is received. The handler processes the received message's header,
     * payload, and client endpoint information.
     *
     * @param messageType The GDTP message type for which the handler is being set.
     * @param handler The function that will process the message. The handler function takes three arguments:
     * - `const GDTPHeader& header`: The header of the received message.
     * - `const std::vector<uint8_t>& payload`: The payload of the received message.
     * - `const asio::ip::udp::endpoint& client_endpoint`: The endpoint of the client that sent the message.
     *
     * @code
     * // Example usage:
     * networkService.setMessageHandler(uint8_t::PlayerMovement, [](const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint) {
     *     // Handle PlayerMovement message
     *     std::cout << "Received PlayerMovement message" << std::endl;
     * });
     * @endcode
     */
    void addEvent(const uint8_t messageType,
        const std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>& handler) const
    {
        (*message_handlers)[messageType] = handler;
    }

    /**
     * @brief Gets the IP address and port the server is currently listening on.
     * @return A string representing the IP address and port in the format "IP:Port".
     */
    [[nodiscard]] std::string getLocalEndpoint() const {
        asio::ip::udp::endpoint local_endpoint = socket_->local_endpoint();
        return local_endpoint.address().to_string() + ":" + std::to_string(local_endpoint.port());
    }

    /**
     * @brief Gets the IP address the server is currently listening on.
     * @return A string representing the IP address.
     */
    [[nodiscard]] std::string getIp() const {
        const asio::ip::udp::endpoint local_endpoint = socket_->local_endpoint();
        return local_endpoint.address().to_string();
    }

    /**
     * @brief Gets the Port the server is currently listening on.
     * @return unsigned short representing the port number.
     */
    [[nodiscard]] uint32_t getPort() const
    {
        return socket_->local_endpoint().port();
    }

    /**
    * @brief Destructor for NetworkingService. Closes the socket and stops the service.
    */
    ~NetworkingService()
    {
        socket_->close();
        stop();
    }

    /**
     * @brief Sends a UDP request to the specified recipient with the given message type and payload.
     *
     * This function constructs a GDTP packet, consisting of a header and an optional payload, and sends it
     * to the specified recipient's IP address and port. The header contains metadata such as the protocol version,
     * message type, packet ID, payload size, and information on packet sequencing. The payload represents the
     * data to be sent, such as game state updates or player actions.
     *
     * @param recipient The IP address of the recipient to which the packet will be sent.
     * @param port The port number on the recipient's side where the packet will be sent.
     * @param messageType The type of the GDTP message to be sent, represented as a `uint8_t`.
     *                    The message type indicates what kind of data the packet contains (e.g., PlayerMovement, PingRequest).
     * @param payload A vector of bytes (`std::vector<uint8_t>`) representing the payload data to be sent with the message.
     *                If no payload is provided, the payload will be empty by default.
     *
     * @note The GDTP packet header is automatically constructed and includes:
     * - Version (1 byte)
     * - Message Type (1 byte)
     * - Packet ID (8 bytes), generated using the current timestamp.
     * - Payload Size (2 bytes)
     * - Sequence Number (2 bytes), defaults to 1 (for unfragmented packets).
     * - Total Packets (2 bytes), defaults to 1 (for unfragmented packets).
     *
     * @note The `messageType` indicates the nature of the message being sent (e.g., Player Movement, Chat, etc.),
     * and the payload provides the data associated with this message.
     *
     * @warning The function does not handle packet fragmentation. If the payload exceeds the size of a typical UDP packet,
     * consider splitting the payload into smaller packets before sending.
     *
     * @code
     * // Example usage:
     * std::vector<uint8_t> payload = {0x01, 0x02, 0x03}; // Payload data
     * networkService.sendRequest("127.0.0.1", 12345, static_cast<uint8_t>(GDTPMessageType::PlayerMovement), payload);
     * @endcode
     *
     * @see sendPacket() for how the constructed packet is sent.
     */
    GDTPHeader sendRequest(
        const std::string& recipient,
        const int port,
        const uint8_t messageType,
        const std::vector<uint8_t>& payload = {}
    ) {
        GDTPHeader header{};
        header.version = 0x01;
        header.messageType = messageType;
        header.packetId = std::chrono::system_clock::now().time_since_epoch().count();
        header.payloadSize = static_cast<uint16_t>(payload.size());
        header.sequenceNumber = 1;
        header.totalPackets = 1;

        std::vector<uint8_t> headerBuffer = header.toBuffer();

        headerBuffer.insert(headerBuffer.end(), payload.begin(), payload.end());

        sendPacket(headerBuffer, recipient, port);
        return header;
    }


    /**
    * @brief Sends a response to a specific recipient using the given header as a basis.
    *
    * This method is designed to send a response message to a specified recipient by reusing
    * the `GDTPHeader` from an initial request. The payload size is automatically adjusted
    * based on the provided payload, and the header's sequence number and total packets
    * are set to default values (1). The method then composes the header and payload
    * into a complete message and sends it using the `sendPacket` method.
    *
    * @param recipient The IP address of the recipient as a string (e.g., "127.0.0.1").
    * @param port The port number of the recipient to which the message should be sent.
    * @param headerOrigin The original `GDTPHeader` from the request, which serves as the basis
    *                     for the response header.
    * @param payload The payload data to be sent with the response. Defaults to an empty vector.
    *
    * @details
    * - The method starts by creating a copy of `headerOrigin` to modify.
    * - The `payloadSize` of the header is updated to match the size of the provided `payload`.
    * - `sequenceNumber` and `totalPackets` are set to `1`, indicating that the message is sent
    *   in a single packet.
    * - The method then converts the header into a byte buffer using `toBuffer()` and appends
    *   the `payload` to this buffer.
    * - Finally, the combined header and payload are sent to the specified recipient using the
    *   `sendPacket` method.
    *
    * @note The header's version and message type are preserved from the `headerOrigin`, allowing
    *       the response to maintain consistency with the original request.
    *
    * @throws std::runtime_error if the `sendPacket` method encounters an error during transmission.
    *
    * @code
    * GDTPHeader originalHeader = ...; // Received or created earlier
    * std::vector<uint8_t> responsePayload = { ... }; // Data to include in the response
    *
    * sendRequestResponse("192.168.1.100", 8080, originalHeader, responsePayload);
    * // This sends a response using the header information from `originalHeader` and includes
    * // `responsePayload` as the data.
    * @endcode
    */
    GDTPHeader sendRequestResponse(
        const std::string& recipient,
        const int port,
        const GDTPHeader& headerOrigin,
        const std::vector<uint8_t>& payload = {}
    ) {
        GDTPHeader header = headerOrigin;
        header.payloadSize = static_cast<uint16_t>(payload.size());
        header.sequenceNumber = 1;
        header.totalPackets = 1;

        std::vector<uint8_t> headerBuffer = header.toBuffer();

        headerBuffer.insert(headerBuffer.end(), payload.begin(), payload.end());

        sendPacket(headerBuffer, recipient, port);
        return header;
    }


    /**
    * @brief Sends a response to a specific recipient using the given header as a basis.
    *
    * This method is designed to send a response message to a specified recipient by reusing
    * the `GDTPHeader` from an initial request. The payload size is automatically adjusted
    * based on the provided payload, and the header's sequence number and total packets
    * are set to default values (1). The method then composes the header and payload
    * into a complete message and sends it using the `sendPacket` method.
    *
    * @param client_endpoint The UDP endpoint of the client, which includes the IP address and port.
    * @param headerOrigin The original `GDTPHeader` from the request, which serves as the basis
    *                     for the response header.
    * @param payload The payload data to be sent with the response. Defaults to an empty vector.
    *
    * @details
    * - The method starts by creating a copy of `headerOrigin` to modify.
    * - The `payloadSize` of the header is updated to match the size of the provided `payload`.
    * - `sequenceNumber` and `totalPackets` are set to `1`, indicating that the message is sent
    *   in a single packet.
    * - The method then converts the header into a byte buffer using `toBuffer()` and appends
    *   the `payload` to this buffer.
    * - Finally, the combined header and payload are sent to the specified recipient using the
    *   `sendPacket` method.
    *
    * @note The header's version and message type are preserved from the `headerOrigin`, allowing
    *       the response to maintain consistency with the original request.
    *
    * @throws std::runtime_error if the `sendPacket` method encounters an error during transmission.
    *
    * @code
    * GDTPHeader originalHeader = ...; // Received or created earlier
    * std::vector<uint8_t> responsePayload = { ... }; // Data to include in the response
    *
    * sendRequestResponse("192.168.1.100", 8080, originalHeader, responsePayload);
    * // This sends a response using the header information from `originalHeader` and includes
    * // `responsePayload` as the data.
    * @endcode
    */
    GDTPHeader sendRequestResponse(
        const asio::ip::udp::endpoint& client_endpoint,
        const GDTPHeader& headerOrigin,
        const std::vector<uint8_t>& payload = {}
    ) {
        return sendRequestResponse(client_endpoint.address().to_string(), client_endpoint.port(), headerOrigin, payload);
    }

    /**
     * @brief Sends a request to a client over UDP using the given endpoint.
     *
     * This function sends a message to the specified client using their UDP endpoint. The message is composed of a header
     * and an optional payload. The header contains the message type, and the payload can include additional data as needed.
     *
     * @param client_endpoint The UDP endpoint of the client, which includes the IP address and port.
     * @param messageType The type of the message being sent. This is an 8-bit unsigned integer that identifies the type of the message.
     * @param payloads An optional vector of bytes that represents the payload to send along with the request. By default,
     * this is an empty vector if no payload is provided.
     *
     * @note This function calls an overloaded version of `sendRequest` that takes the client's IP address as a string
     * and their port number as arguments. It simplifies the process by allowing the user to pass an ASIO UDP endpoint directly.
     *
     * @code
     * asio::ip::udp::endpoint clientEndpoint(asio::ip::address::from_string("127.0.0.1"), 8080);
     * uint8_t messageType = 1; // Example message type
     * std::vector<uint8_t> payload = {0x01, 0x02, 0x03}; // Example payload
     * sendRequest(clientEndpoint, messageType, payload);
     * @endcode
     */
    GDTPHeader sendRequest(
        const asio::ip::udp::endpoint& client_endpoint,
        const uint8_t messageType,
        const std::vector<uint8_t>& payloads = {}
    ) {
        return sendRequest(client_endpoint.address().to_string(), client_endpoint.port(), messageType, payloads);
    }


    /**
     * @brief Starts the asynchronous reception of UDP packets.
     *
     * This method sets up the `NetworkingService` to continuously listen for incoming UDP packets.
     * It uses the ASIO library's `async_receive_from` function to asynchronously receive data from
     * remote endpoints without blocking the execution.
     *
     * @details
     * - The method sets up a buffer (`recv_buffer_`) to store the incoming packet data and listens
     *   for packets from any client, storing the sender's endpoint in `remote_endpoint_`.
     * - Once a packet is received, it triggers a lambda function that checks if the reception
     *   was successful (i.e., no errors and non-zero bytes received).
     * - If successful, the method calls `handleReceivedPacket()` to process the received data.
     * - After processing, it immediately calls `startReceive()` again to continue listening for
     *   more incoming packets, thus ensuring a continuous reception loop.
     *
     * @param ec The error code passed by the ASIO callback, indicating if an error occurred
     *        during packet reception.
     * @param bytes_recvd The number of bytes received in the packet.
     *
     * @note This method should be called once during the initialization to start the reception
     *       loop. It ensures that the service remains ready to receive data at all times.
     *
     * @warning Since this method uses asynchronous operations, it is crucial that the `io_context`
     *          is running (e.g., by calling `run()`) to process incoming events.
     *
     * @code
     * // Example usage:
     * NetworkingService& networkService = NetworkingService::getInstance();
     * networkService.startReceive(); // Start listening for incoming UDP packets.
     * networkService.run();          // Start the I/O context in a separate thread.
     * @endcode
     *
     * @see `handleReceivedPacket()` for processing the received data.
     * @see `run()` for starting the I/O context in a separate thread.
     */
    void startReceive() {
        socket_->async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            [this](const std::error_code ec, const std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    std::cout << "Received " << bytes_recvd << " bytes from " << remote_endpoint_ << std::endl;
                    handleReceivedPacket(recv_buffer_, bytes_recvd, remote_endpoint_);
                }
                startReceive(); // Continue listening for more packets.
            }
        );
    }

    /**
     * @brief Initializes the networking service by setting up the UDP socket.
     *
     * This function sets up the UDP socket for the networking service. It ensures that the
     * socket is only initialized once by using a static `isInit` flag. If the socket has already
     * been initialized, subsequent calls to this function will have no effect.
     *
     * @details
     * - The function checks if the `isInit` flag is `false`, indicating that the socket has not been initialized.
     * - It then attempts to create a new `asio::ip::udp::socket` using the provided I/O context and binds it
     *   to a UDP endpoint with the specified port.
     * - If the initialization is successful, `isInit` is set to `true` to prevent future reinitializations.
     * - If an exception occurs during socket creation, it is caught and the error message is printed to `std::cerr`.
     *
     * @note This function is automatically called inside the `run()` method if it has not been called before.
     *       Therefore, it is not strictly necessary to call `init()` manually unless you want to initialize
     *       the socket explicitly before starting the service.
     *
     * @code
     * // Example usage:
     * NetworkingService& networkService = NetworkingService::getInstance();
     * networkService.init(); // Explicitly initialize the network service.
     * networkService.run();  // Will not attempt to reinitialize if already initialized.
     * @endcode
     *
     * @throws std::runtime_error If an error occurs during the creation of the socket, the exception is caught
     *         and logged, but the function does not rethrow the exception. This allows the caller to handle
     *         the error gracefully.
     */
    void init()
    {
        static bool isInit = false;

        if (!isInit)
        {
            try {
                socket_ = asio::ip::udp::socket(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), _port));
                isInit = true;
                startReceive();
                std::cout << "Network service initialized on port : " << _port << std::endl;
            } catch (std::exception &e) {
                std::cerr << "Error in Network service: " << e.what() << std::endl;
                throw std::runtime_error("Failed to initialize network service");
            }
        }
    }

    /**
     * @brief Runs the NetworkingService in a separate thread.
     *
     * This method initiates the ASIO I/O context in a new thread, allowing the `NetworkingService`
     * to perform asynchronous network operations without blocking the main thread.
     *
     * @details
     * - The method creates a new `std::jthread`, which automatically manages thread creation and
     *   cleanup when the thread is no longer needed.
     * - Inside the thread, it calls `io_context_.run()`, which processes incoming events,
     *   such as network operations, asynchronously.
     * - Using a separate thread ensures that the network service can continue to send, receive,
     *   and process packets in the background, enabling the main thread to perform other tasks
     *   without being blocked by network operations.
     *
     * @note The `std::jthread` used here is automatically joined upon destruction, making it
     *       easier to manage thread lifecycle without the need for explicit `join()` calls.
     *
     * @warning It is important to call `stop()` before the `NetworkingService` is destroyed
     *          to ensure that the `io_context` is properly stopped, and the thread can terminate safely.
     *
     * @code
     * // Example usage:
     * NetworkingService& networkService = NetworkingService::getInstance();
     * networkService.startReceive(); // Set up asynchronous packet reception.
     * networkService.run();          // Start the I/O context in a new thread.
     * @endcode
     *
     * @see `startReceive()` for initiating the reception of incoming packets.
     * @see `stop()` for stopping the I/O context and thread safely.
     */
    void run() {
        init();
        thread = std::jthread([this] {
            io_context_.run();
            std::cout << "Networking service thread started" << std::endl;
        });

    }


    /**
     * @brief Stops the NetworkingService and terminates the associated thread.
     *
     * This method gracefully stops the `NetworkingService` by halting its `asio::io_context` and
     * signaling the thread running the service to stop. It ensures that the ongoing network operations
     * are terminated safely and prevents further processing.
     *
     * @details
     * - The `io_context_.stop()` call stops any asynchronous operations, effectively ending the
     *   event loop used by `asio` for networking.
     * - The `request_stop()` function on the `std::jthread` object requests the thread running the
     *   service to stop its execution.
     * - This method should be called before the application terminates or when the network service
     *   needs to be shut down to avoid leaving the network thread running.
     *
     * @note This method does not immediately terminate the thread. Instead, it requests the
     *       termination, allowing any cleanup operations to complete.
     *
     * @code
     * // Example usage:
     * NetworkingService& networkService = NetworkingService::getInstance();
     * networkService.stop();
     * @endcode
     *
     * @see `attempt()` for waiting until the thread has fully terminated.
     */
    void stop()
    {
        io_context_.stop();
        thread.request_stop();
    }


    /**
     * @brief Blocks the calling thread until the network service thread finishes execution.
     *
     * This method waits for the thread running the `NetworkingService` to finish its operations.
     * It is typically used after calling `stop()` to ensure that the networking thread has
     * completely stopped before proceeding.
     *
     * @details
     * - The method calls `join()` on the `std::jthread` object, which blocks the calling thread
     *   until the `NetworkingService` thread completes.
     * - This is useful when you need to ensure that the network thread has fully terminated before
     *   proceeding with other tasks, such as application shutdown or cleanup.
     * - If the thread is already finished, `join()` returns immediately.
     *
     * @warning Failing to call `attempt()` after `stop()` may result in the thread being left in
     *          an unfinished state, which could lead to undefined behavior during shutdown.
     *
     * @code
     * // Example usage:
     * NetworkingService& networkService = NetworkingService::getInstance();
     * networkService.stop();   // Request to stop the network operations.
     * networkService.attempt(); // Wait until the network thread finishes.
     * @endcode
     *
     * @throws std::system_error If the thread cannot be joined, a `std::system_error` exception may be thrown.
     *
     * @see `stop()` for initiating the network service shutdown process.
     */
    void attempt()
    {
        thread.join();
    }


    /**
     * @brief Exception class for handling unknown payload messages.
     */
    class UnknownPayloadMessage : public std::exception {
    private:
        uint8_t messageType;

    public:
        UnknownPayloadMessage() = delete;

        explicit UnknownPayloadMessage(const uint8_t messageType)
            : messageType(messageType) {}

        [[nodiscard]] const char *what() const noexcept override
        {
            return &"Payload Message handlers is missing for this message type: " [ static_cast<int>(this->messageType)];
        }
    };

private:
    asio::io_context io_context_;                   ///< ASIO I/O context for handling asynchronous network operations.
    std::optional<asio::ip::udp::socket> socket_;                  ///< ASIO UDP socket for sending and receiving packets.
    int _port;                                      ///< Port number for the server to listen on.
    asio::ip::udp::endpoint remote_endpoint_;       ///< Endpoint of the remote client sending the packet.
    std::array<uint8_t, 1400> recv_buffer_{};         ///< Buffer for receiving incoming packets.
    std::jthread thread;                            ///< Thread for running the ASIO I/O context.
    std::shared_ptr<std::map<uint8_t, std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>>> message_handlers; ///< Handlers for processing received messages.

    /**
     * @brief Sends a UDP packet to the specified recipient.
     *
     * This method is responsible for transmitting a UDP packet to a given recipient's IP address and port.
     * It constructs the appropriate endpoint from the provided address and port and attempts to send
     * the packet using the `asio` library's `send_to` function.
     *
     * @param packet A `std::vector<uint8_t>` containing the packet data, which typically includes a serialized
     *               header followed by the payload data. The data in the vector will be transmitted as-is
     *               over the network.
     * @param recipient A `std::string` representing the IP address of the recipient. This can be in IPv4
     *                  or IPv6 format, depending on the target's address.
     * @param port An `int` representing the UDP port number of the recipient. This port is where the recipient
     *             is expected to listen for incoming UDP packets.
     *
     * @details
     * - The method constructs an `asio::ip::udp::endpoint` using the recipient's IP address and port.
     * - It then calls `socket_.send_to()` with the constructed endpoint and the packet data, wrapped in an
     *   `asio::buffer`. If the operation fails, it captures the error code.
     * - In case of an error during the send operation, the method throws a `std::runtime_error` with a
     *   descriptive error message, making it easier to debug connection issues.
     * - The method ensures that the packet is sent asynchronously using `asio` to avoid blocking the main thread.
     *
     * @code
     * // Example usage in a NetworkingService class:
     * std::vector<uint8_t> packetData = ...; // Prepare packet with header and payload
     * std::string recipientIP = "192.168.1.100";
     * int recipientPort = 12345;
     *
     * try {
     *     sendPacket(packetData, recipientIP, recipientPort);
     *     std::cout << "Packet sent successfully." << std::endl;
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Failed to send packet: " << e.what() << std::endl;
     * }
     * @endcode
     *
     * @throws std::runtime_error If the `send_to` operation fails, an exception is thrown with the
     *         error message provided by `asio`. This exception should be caught by the caller to
     *         handle network errors gracefully.
     *
     * @warning Ensure that the recipient IP address and port are correct before calling this method.
     *          An invalid address or port can lead to exceptions or failed transmissions.
     *
     * @see `asio::ip::udp::endpoint` for more information about constructing network endpoints.
     * @see `asio::buffer` for details on how data is wrapped for network operations.
     */
    void sendPacket(
        const std::vector<uint8_t>& packet,
        const std::string& recipient,
        const int port
    ) {
        const asio::ip::udp::endpoint recipient_endpoint(asio::ip::address::from_string(recipient), port);
        std::error_code ec;
        socket_->send_to(asio::buffer(packet), recipient_endpoint, 0, ec);

        if (ec) {
            std::cout << "Failed to send packet: " << ec.message() << std::endl;
        }
    }


    /**
     * @brief Handles the reception of a packet and processes its content.
     *
     * This method processes incoming UDP packets by verifying their integrity, extracting
     * the header and payload, and passing them to the appropriate message processing function.
     * It ensures that the received data is correctly formatted before delegating the message
     * to the `processMessage` function.
     *
     * @param packet A `std::array<uint8_t, 1400>` containing the raw data received from the network.
     *               This array represents the entire packet data, which includes both the header
     *               and the payload.
     * @param length The size of the received packet in bytes (`std::size_t`). It indicates how many
     *               bytes of valid data are present in the `packet` array.
     * @param client_endpoint An `asio::ip::udp::endpoint` representing the network address and port
     *                        of the client that sent the packet. It can be used to send responses
     *                        back to the client or to log the source of the packet.
     *
     * @details
     * - This function begins by checking if the packet size is at least as large as the header size.
     *   If the packet is smaller than `HEADER_SIZE`, it logs an error and returns early.
     * - It then converts the packet data into a `std::vector<uint8_t>` for easier handling and extracts
     *   the header using the `GDTPHeader::fromBuffer` method.
     * - After parsing the header, it verifies that the packet contains the expected payload size.
     *   If the length of the packet is less than the header size plus the payload size, it logs an
     *   error indicating an incorrect payload size and returns.
     * - Once the packet is validated, the payload is extracted from the vector, and the `processMessage`
     *   function is called with the `header`, `payload`, and `client_endpoint`.
     *
     * @code
     * // Example usage in a NetworkingService class:
     * std::array<uint8_t, 1400> receivedPacket;
     * std::size_t packetLength = ...; // Length of data received
     * asio::ip::udp::endpoint senderEndpoint;
     *
     * handleReceivedPacket(receivedPacket, packetLength, senderEndpoint);
     * @endcode
     *
     * @warning This method assumes that `HEADER_SIZE` is a defined constant representing the size of the
     *          GDTP header. It is crucial that this size matches the actual structure size of `GDTPHeader`.
     *
     * @throws std::runtime_error If the `GDTPHeader::fromBuffer` method fails or encounters invalid data,
     *         an exception might be thrown depending on its implementation.
     *
     * @see `GDTPHeader::fromBuffer` for more information on how headers are parsed.
     * @see `processMessage` for details on how the extracted message is processed.
     */
    void handleReceivedPacket(
        const std::array<uint8_t, 1400>& packet,
        const std::size_t length,
        const asio::ip::udp::endpoint& client_endpoint
    ) const {
        if (length < HEADER_SIZE) {
            std::cerr << "Received malformed packet: insufficient header size" << std::endl;
            return;
        }

        std::vector packetData(packet.begin(), packet.begin() + length);
        const GDTPHeader header = GDTPHeader::fromBuffer(packetData);
        if (static_cast<uint16_t>(length) < HEADER_SIZE + header.payloadSize) {
            std::cerr << "Received malformed packet: incorrect payload size" << std::endl;
            return;
        }

        const std::vector payload(packetData.begin() + HEADER_SIZE, packetData.begin() + HEADER_SIZE + header.payloadSize);

        processMessage(header.messageType, payload, header, client_endpoint);
    }


    /**
     * @brief Processes the received message based on its type.
     *
     * This method handles the processing of a received GDTP message by using the provided
     * message type to identify and call the corresponding message handler function.
     * It checks if a handler exists for the specified message type, and if so,
     * it invokes the handler with the provided message details.
     *
     * @param messageType The type of the received GDTP message as a `uint8_t`. This is used
     *                    to determine which handler to call. It corresponds to different
     *                    types of game events or protocol messages.
     * @param payload A `std::vector<uint8_t>` containing the payload data of the message.
     *                This represents the actual data that was sent along with the message,
     *                such as player position, chat message, or other game-related information.
     * @param header The `GDTPHeader` of the received message. It contains metadata about the message,
     *               including its version, message type, packet ID, and payload size.
     *               This information is essential for understanding the context of the message.
     * @param client_endpoint An `asio::ip::udp::endpoint` representing the endpoint of the client
     *                        that sent the message. It includes the client's IP address and port number,
     *                        which can be used for responding or tracking the sender.
     *
     * @details
     * - The method first checks if a handler exists for the given `messageType` using the `message_handlers` map.
     * - If a handler is found, it calls the handler function, passing the `header`, `payload`,
     *   and `client_endpoint` as arguments. This allows the handler to process the data appropriately.
     * - If no handler is registered for the `messageType`, an error message is logged to `std::cerr`
     *   indicating that no handler was found for the specified message type.
     * - This method is typically used to decouple the reception of messages from their processing,
     *   allowing different message types to be handled by different functions or classes.
     *
     * @note This method assumes that `message_handlers` is a `std::shared_ptr` to a map that
     *       contains functions for handling various message types.
     *       Each function is expected to have a signature of
     *       `void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)`.
     *
     * @throws std::runtime_error If a handler is invoked and encounters an error internally.
     *
     * @code
     * // Example usage:
     * std::vector<uint8_t> payload = ...; // Received payload data
     * GDTPHeader header = ...; // Received header with metadata
     * asio::ip::udp::endpoint client_endpoint = ...; // Client's endpoint
     *
     * // Process a PlayerMovement message with messageType = 0x01
     * processMessage(0x01, payload, header, client_endpoint);
     * @endcode
     *
     * @warning Ensure that the `message_handlers` map is properly initialized and contains handlers
     *          for all expected message types to avoid missing important messages.
     *
     * @see GDTPHeader for more information about the header structure.
     */
    void processMessage(
        const uint8_t messageType,
        const std::vector<uint8_t>& payload,
        const GDTPHeader& header,
        const asio::ip::udp::endpoint& client_endpoint
    ) const {
        if (message_handlers->contains(messageType)  ) {
            (*message_handlers)[messageType](header, payload, client_endpoint);
        } else {
            std::cerr << "No handler found for message type: " << static_cast<int>(messageType) << std::endl;
        }
    }

};

#endif // NETWORKSERVICE_HPP
