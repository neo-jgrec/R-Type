/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkService.hpp
*/
#pragma once

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
    #include "./includes/RequestType.hpp"
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
        static NetworkingService instance(server_ip, port, message_handlers, payload_handlers);
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
        const int port,
        const std::shared_ptr<std::map<uint8_t, std::function<void(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint)>>>& message_handlers = Receive::handlersMap(),
        const std::shared_ptr<std::map<uint8_t, std::function<std::vector<uint8_t>(std::vector<std::string>)>>>& payload_handlers = Payload::payloadMap()
    )
        : socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), message_handlers(message_handlers), payload_handlers(payload_handlers) {
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
    void setMessageHandler(const uint8_t messageType,
                           const std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>& handler) {
        (*message_handlers)[messageType] = handler;
    }

    /**
 * @brief Sets the handler function for preparing the payload for a specific GDTP message type.
 *
 * This method allows the user to assign a function that will prepare the payload
 * for a specific GDTP message type before sending. The function is responsible for converting
 * the input arguments into a binary payload that matches the message type specification.
 *
 * @param messageType The GDTP message type for which the payload handler is being set.
 * @param handler The function that will generate the payload. The handler function takes a single argument:
 * - `std::vector<std::string> args`: A vector of arguments (as strings) to be converted into a payload.
 * The function returns a `std::vector<uint8_t>` representing the binary payload.
 *
 * @code
 * // Example usage:
 * networkService.setPayloadHandler(uint8_t::PlayerMovement, [](std::vector<std::string> args) -> std::vector<uint8_t> {
 *     std::vector<uint8_t> payload;
 *     // Prepare payload from args...
 *     return payload;
 * });
 * @endcode
 */
    void setPayloadHandler(const uint8_t messageType, std::function<std::vector<uint8_t>(std::vector<std::string>)> &handler) {
        (*payload_handlers)[messageType] = handler;
    }

    /**
     * @brief Sets the shared pointer to the map of message handlers.
     *
     * This function allows setting a shared pointer to a map where each GDTP message type is associated
     * with a handler function. The handler functions process incoming messages of the given type.
     * The existing pointer to the handlers map is replaced by the new one.
     *
     * Each handler function takes the following parameters:
     * - `const GDTPHeader& header`: The header of the received message.
     * - `const std::vector<uint8_t>& payload`: The payload of the received message.
     * - `const asio::ip::udp::endpoint& client_endpoint`: The endpoint of the client that sent the message.
     *
     * @param message_handlers A shared pointer to a map that associates GDTP message types (`uint8_t`)
     * with their corresponding handler functions.
     *
     * @note This function replaces the current shared pointer to the message handlers map with the new one provided.
     *
     * @code
     * auto newMessageHandlers = std::make_shared<std::map<uint8_t, std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>>>();
     * newMessageHandlers->emplace(uint8_t::PlayerMovement, [](const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint) {
     *     // Handle PlayerMovement message
     * });
     * networkService.setMessagesHandler(newMessageHandlers);
     * @endcode
     */
    void setMessagesHandler(
        std::shared_ptr<std::map<uint8_t, std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>>> message_handlers
    )
    {
        this->message_handlers = std::move(message_handlers);
    }


    /**
     * @brief Sets the shared pointer to the map of payload preparation handlers.
     *
     * This function allows setting a shared pointer to a map where each GDTP message type is associated
     * with a function that prepares the payload before sending it. The existing pointer to the payload
     * handlers map is replaced by the new one.
     *
     * Each handler function takes the following parameter:
     * - `std::vector<std::string> args`: A vector of string arguments that will be converted into a binary payload.
     *
     * The handler function returns:
     * - `std::vector<uint8_t>`: A binary representation of the payload for the message type.
     *
     * @param payload_handlers A shared pointer to a map that associates GDTP message types (`uint8_t`)
     * with their corresponding payload preparation functions.
     *
     * @note This function replaces the current shared pointer to the payload handlers map with the new one provided.
     *
     * @code
     * auto newPayloadHandlers = std::make_shared<std::map<uint8_t, std::function<std::vector<uint8_t>(std::vector<std::string>)>>>();
     * newPayloadHandlers->emplace(uint8_t::PlayerMovement, [](std::vector<std::string> args) -> std::vector<uint8_t> {
     *     // Prepare the payload for PlayerMovement
     *     return std::vector<uint8_t>;
        * });
        * networkService.setPayloadsHandler(newPayloadHandlers);
        * @endcode
        */
void setPayloadsHandler(
   std::shared_ptr<std::map<uint8_t, std::function<std::vector<uint8_t>(std::vector<std::string>)>>>& payload_handlers
)
{
    this->payload_handlers = std::move(payload_handlers);
}


    /**
    * @brief Destructor for NetworkingService. Closes the socket and stops the service.
    */
    ~NetworkingService() {
        socket_.close();
        this->stop();
    }

    /**
     * @brief Sends a request to a specified recipient with the given message type and arguments.
     * @param recipient The IP address of the recipient.
     * @param port The port number of the recipient.
     * @param messageType The type of the GDTP message to be sent.
     * @param args Optional arguments for the payload of the message.
     */
    void sendRequest(
        const std::string& recipient,
        const int port,
        uint8_t messageType,
        const std::vector<std::string>& args = {}
    ) {
        std::vector<uint8_t> payload = preparePayload(messageType, args);
        std::cout << "Payload size: " << payload.size() << std::endl;

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
     * @brief Sends a request to a client using its endpoint.
     * @param client_endpoint The UDP endpoint of the client (contains IP and port).
     * @param messageType The type of the GDTP message to be sent.
     * @param args Optional arguments for the payload of the message.
     */
    void sendRequest(
        const asio::ip::udp::endpoint& client_endpoint,
        const uint8_t messageType,
        const std::vector<std::string>& args = {}
    ) {
        sendRequest(client_endpoint.address().to_string(), client_endpoint.port(), messageType, args);
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
     std::shared_ptr<std::map<uint8_t, std::function<std::vector<uint8_t>(std::vector<std::string>)>>> payload_handlers; ///< Handlers for preparing message payloads.

    /**
    * @brief Prepares the payload based on the GDTP message type and arguments.
    * @param messageType The type of the GDTP message.
    * @param args The arguments required for the message's payload.
    * @return A vector of bytes representing the payload.
    */
    std::vector<uint8_t> preparePayload(
        const uint8_t messageType,
        const std::vector<std::string>& args) const {
        std::vector<uint8_t> payload;
        if (this->payload_handlers->contains(messageType)) {
            payload = (*this->payload_handlers)[messageType](args);
        } else {
            throw UnknownPayloadMessage(messageType);
        }
        return payload;
    }

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
            this->sendRequest(client_endpoint, static_cast<uint8_t>(GDTPMessageType::ConnectionReject), { "Invalid packet" });
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

    /**
     * @brief Sends a "Connection Accept" message to the client.
     * @param client_endpoint The endpoint of the client to which the message is sent.
     */
    void sendConnectionAccept(
        const asio::ip::udp::endpoint& client_endpoint
    ) {
        GDTPHeader header{};
        header.version = 0x01; // Version du protocole
        header.messageType = static_cast<uint8_t>(GDTPMessageType::ConnectionAccept); // Type de message
        header.packetId = std::chrono::system_clock::now().time_since_epoch().count(); // Utilisation d'un timestamp comme ID
        header.payloadSize = 0; // Pas de payload pour Connection Accept

        std::vector<uint8_t> headerBuffer = header.toBuffer();

        std::error_code ec;
        socket_.send_to(asio::buffer(headerBuffer), client_endpoint, 0, ec);

        if (ec) {
            std::cerr << "Failed to send Connection Accept: " << ec.message() << std::endl;
        } else {
            std::cout << "Connection Accept sent to " << client_endpoint.address().to_string()
                      << ":" << client_endpoint.port() << std::endl;
        }
    }
};

#endif // NETWORKSERVICE_HPP
