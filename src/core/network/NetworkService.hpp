/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkService.hpp
*/
#pragma once

#ifndef NETWORKSERVICE_HPP
    #define NETWORKSERVICE_HPP329
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
    #include "../game/PreparePayload.hpp"
    #include "../game/HandlersReceive.hpp"


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
 * @param server_ip The IP address for the server (default is "127.0.0.1").
 * @param port The port number for the server (default is 12345).
 * @return A reference to the singleton instance of NetworkingService.
 *
 * @note The IP address and port are used only during the first call to this method. Subsequent calls
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
        const std::string& server_ip = "127.0.0.1",
        int port = 12345,
        const std::shared_ptr<std::map<uint8_t, std::function<void(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint)>>>& message_handlers = Receive::handlersMap(),
        const std::shared_ptr<std::map<uint8_t, std::function<std::vector<uint8_t>(std::vector<std::string>)>>>& payload_handlers = Payload::payloadMap()

    ) {
        static NetworkingService instance(server_ip, port);
        return instance;
    }

    // Interdire la copie et l'affectation de l'instance
    NetworkingService(const NetworkingService&) = delete;
    NetworkingService& operator=(const NetworkingService&) = delete;
    /**
    * @brief Constructs a NetworkingService object and starts receiving packets on the specified IP and port.
    * @param server_ip The IP address of the server.
    * @param port The port number to listen on.
    * @param message_handlers A shared pointer to a map of message handlers for different uint8_t.
    * @param payload_handlers A shared pointer to a map of payload handlers for different uint8_t.
    */
    NetworkingService(
        const std::string& server_ip,
        const int port
    )
        : socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
        std::cout << "Server listening on " << server_ip << ":" << port << std::endl;
        startReceive(); // Commence l'écoute des paquets dès l'initialisation
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
                           const std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>& handler) {
        (*message_handlers)[messageType] = handler;
    }

    /**
 * @brief Gets the IP address and port the server is currently listening on.
 * @return A string representing the IP address and port in the format "IP:Port".
 */
    std::string getLocalEndpoint() const {
        asio::ip::udp::endpoint local_endpoint = socket_.local_endpoint();
        return local_endpoint.address().to_string() + ":" + std::to_string(local_endpoint.port());
    }

    /**
     * @brief Gets the IP address the server is currently listening on.
     * @return A string representing the IP address.
     */
    std::string getIp() const {
        asio::ip::udp::endpoint local_endpoint = socket_.local_endpoint();
        return local_endpoint.address().to_string();
    }

    /**
     * @brief Gets the Port the server is currently listening on.
     * @return unsigned short representing the port number.
     */
    unsigned short getPort() const
    {
        return socket_.local_endpoint().port();
    }

    /**
    * @brief Destructor for NetworkingService. Closes the socket and stops the service.
    */
    ~NetworkingService() {
        socket_.close();
        this->stop();
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
void sendRequest(
    const std::string& recipient,
    const int port,
    uint8_t messageType,
    const std::vector<uint8_t>& payload = {}
) {

        GDTPHeader header{};
        header.version = 0x01;
        header.messageType = static_cast<uint8_t>(messageType);

        header.packetId = std::chrono::system_clock::now().time_since_epoch().count();

        header.payloadSize = static_cast<uint16_t>(payload.size());

        header.sequenceNumber = 1;
        header.totalPackets = 1;

        std::vector<uint8_t> headerBuffer = header.toBuffer();

        headerBuffer.insert(headerBuffer.end(), payload.begin(), payload.end());

        sendPacket(headerBuffer, recipient, port);
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
    void sendRequest(
        const asio::ip::udp::endpoint& client_endpoint,
        const uint8_t messageType,
        const std::vector<uint8_t>& payloads = {}
    ) {
        sendRequest(client_endpoint.address().to_string(), client_endpoint.port(), messageType, payloads);
    }


    /**
     * @brief Starts the asynchronous reception of UDP packets.
     *
     * This method continuously listens for incoming packets using the ASIO library's async_receive_from function.
     */
    void startReceive() {
        socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            [this](const std::error_code ec, const std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    handleReceivedPacket(recv_buffer_, bytes_recvd, remote_endpoint_);
                }
                startReceive(); // Réécoute pour le prochain paquet
            }
        );
    }

    /**
     * @brief Runs the NetworkingService in a separate thread.
     *
     * This method starts the ASIO I/O context in a separate thread, allowing the service to handle network I/O asynchronously.
     */
    void run() {
        this->thread = std::jthread([this]() {
            io_context_.run();
        });
    }

    /**
     * @brief Stops the NetworkingService and terminates the associated thread.
     *
     * Stops the ASIO I/O context and requests the termination of the thread running the service.
     */
    void stop()
    {
        io_context_.stop();
        this->thread.request_stop();
    }

    /**
    * @brief Blocks the calling thread until the network service thread finishes execution.
    *
    * This method waits for the network service thread to complete, ensuring all network operations have finished.
    */
    void attempt()
    {
        thread.join();
    }

    //create errror for unkown payload message
    /**
     * @brief Exception class for handling unknown payload messages.
     */
    class UnknownPayloadMessage : public std::exception {
        public:
            UnknownPayloadMessage() = delete;
            UnknownPayloadMessage(uint8_t messageType) : messageType(messageType)
            {

            }
            const char *what() const noexcept override
            {
                return "Payload Message handlers is missing for this message type: " + static_cast<int>(this->messageType);
            };
        private:
            uint8_t messageType;
    };
private:
    asio::io_context io_context_;                   ///< ASIO I/O context for handling asynchronous network operations.
    asio::ip::udp::socket socket_;                  ///< ASIO UDP socket for sending and receiving packets.
    asio::ip::udp::endpoint remote_endpoint_;       ///< Endpoint of the remote client sending the packet.
    std::array<uint8_t, 1400> recv_buffer_{};         ///< Buffer for receiving incoming packets.
    std::jthread thread;                            ///< Thread for running the ASIO I/O context.
    std::shared_ptr<std::map<uint8_t, std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>>> message_handlers; ///< Handlers for processing received messages.

    /**
     * @brief Sends a UDP packet to the specified recipient.
     * @param packet The packet data to be sent (header + payload).
     * @param recipient The IP address of the recipient.
     * @param port The port number of the recipient.
     */
    void sendPacket(
        const std::vector<uint8_t>& packet,
        const std::string& recipient,
        const int port
    ) {
        const asio::ip::udp::endpoint recipient_endpoint(asio::ip::address::from_string(recipient), port);
        std::error_code ec;
        const size_t bytesSent = socket_.send_to(asio::buffer(packet), recipient_endpoint, 0, ec);

        if (ec) {
            throw std::runtime_error("Failed to send packet: " + ec.message());
        }

        std::cout << "Packet successfully sent to " << recipient
                  << " (" << bytesSent << " bytes)" << std::endl;
    }

    /**
     * @brief Handles the reception of a packet and processes its content.
     * @param packet The received packet data.
     * @param length The length of the received packet.
     * @param client_endpoint The endpoint of the client that sent the packet.
     */
    void handleReceivedPacket(
        const std::array<uint8_t, 1400>& packet,
        const std::size_t length,
        const asio::ip::udp::endpoint& client_endpoint
    ) {
        std::cout << "Received packet of length: " << length << std::endl;

        if (length < HEADER_SIZE) {
            std::cerr << "Received malformed packet: insufficient header size" << std::endl;
            return;
        }

        std::vector packetData(packet.begin(), packet.begin() + length);

        GDTPHeader header = GDTPHeader::fromBuffer(packetData);

        std::cout << "Packet details - Version: " << static_cast<int>(header.version)
                  << ", Message Type: " << static_cast<int>(header.messageType)
                  << ", Packet ID: " << header.packetId
                  << ", Payload Size: " << header.payloadSize
                  << ", Sequence Number: " << header.sequenceNumber
                  << ", Total Packets: " << header.totalPackets
                    << std::endl;
        if (static_cast<uint16_t>(length) < HEADER_SIZE + header.payloadSize) {
            std::cerr << "Received malformed packet: incorrect payload size" << std::endl;
            return;
        }

        const std::vector payload(packetData.begin() + HEADER_SIZE, packetData.begin() + HEADER_SIZE + header.payloadSize);

        processMessage(static_cast<uint8_t>(header.messageType), payload, header, client_endpoint);
    }

    /**
     * @brief Processes the received message based on its type.
     * @param messageType The type of the received GDTP message.
     * @param payload The payload of the message.
     * @param header The header of the received message.
     * @param client_endpoint The endpoint of the client that sent the message.
     */
    void processMessage(
        uint8_t messageType,
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
