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


/**
 * @class NetworkingService
 * @brief Provides networking services for sending and receiving UDP packets, handling various types of GDTP messages.
 *
 * The NetworkingService class handles UDP-based client-server communications, including sending requests, receiving packets,
 * and processing messages based on GDTP message types. It uses the ASIO library for asynchronous network I/O.
 */
class NetworkingService {
public:
    /**
    * @brief Constructs a NetworkingService object and starts receiving packets on the specified IP and port.
    * @param server_ip The IP address of the server.
    * @param port The port number to listen on.
    * @param message_handlers A shared pointer to a map of message handlers for different GDTPMessageType.
    */
    NetworkingService(
        const std::string& server_ip,
        const int port,
        const std::shared_ptr<std::map<GDTPMessageType, std::function<void(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint)>>>& message_handlers = std::make_shared<std::map<GDTPMessageType, std::function<void(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint)>>>()
    )
        : socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), message_handlers(message_handlers) {
        std::cout << "Server listening on " << server_ip << ":" << port << std::endl;
        startReceive(); // Commence l'écoute des paquets dès l'initialisation
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
        GDTPMessageType messageType,
        const std::vector<std::string>& args = {}
    ) {
        std::vector<uint8_t> payload = preparePayload(messageType, args);
        std::cout << "Payload size: " << payload.size() << std::endl;

        // Création du header en utilisant GDTPHeader
        GDTPHeader header{};
        header.version = 0x01; // Version du protocole
        header.messageType = static_cast<uint8_t>(messageType); // Type de message

        // Packet ID basé sur le timestamp
        header.packetId = std::chrono::system_clock::now().time_since_epoch().count();

        // Taille du payload
        header.payloadSize = static_cast<uint16_t>(payload.size());

        // Conversion du header en buffer
        std::vector<uint8_t> headerBuffer = header.toBuffer();

        // Construction du paquet complet (header + payload)
        headerBuffer.insert(headerBuffer.end(), payload.begin(), payload.end());

        // Envoi du paquet au client destinataire
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
        const GDTPMessageType messageType,
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
        this->thread.join();
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

private:
    asio::io_context io_context_;                   ///< ASIO I/O context for handling asynchronous network operations.
    asio::ip::udp::socket socket_;                  ///< ASIO UDP socket for sending and receiving packets.
    asio::ip::udp::endpoint remote_endpoint_;       ///< Endpoint of the remote client sending the packet.
    std::array<uint8_t, 1400> recv_buffer_{};         ///< Buffer for receiving incoming packets.
    std::jthread thread;                            ///< Thread for running the ASIO I/O context.
    std::shared_ptr<std::map<GDTPMessageType, std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>>> message_handlers; ///< Handlers for processing received messages.

    /**
    * @brief Prepares the payload based on the GDTP message type and arguments.
    * @param messageType The type of the GDTP message.
    * @param args The arguments required for the message's payload.
    * @return A vector of bytes representing the payload.
    */
    static std::vector<uint8_t> preparePayload(
        const GDTPMessageType messageType,
        const std::vector<std::string>& args
    ) {
        std::vector<uint8_t> payload;
        switch (messageType) {
            case GDTPMessageType::PlayerMovement:
                payload = preparePlayerMovement(args);
                break;
            case GDTPMessageType::PlayerShoot:
                payload = preparePlayerShoot(args);
                break;
            case GDTPMessageType::ChatMessage:
                payload = prepareChatMessage(args);
                break;
            case GDTPMessageType::ConnectionRequest: // Ajout de la gestion de la requête de connexion
                payload = prepareConnectionRequest(args);
                break;
            // Ajout d'autres cas pour d'autres types de message
            default:
                std::cerr << "Message type not implemented!" << std::endl;
                break;
        }
        return payload;
    }

    /**
     * @brief Prepares the payload for an error message (ErrorMessage).
     * @param args The arguments required for the error message (error code and description).
     * @return A vector of bytes representing the error message payload.
     */
    static std::vector<uint8_t> prepareErrorMessage(
        const std::vector<std::string>& args
    ) {
        if (args.size() < 2) {
            std::cerr << "Invalid arguments for ErrorMessage!" << std::endl;
            return {};
        }

        const auto errorCode = static_cast<uint8_t>(std::stoi(args[0]));

        const std::string& errorDescription = args[1];

        std::vector<uint8_t> payload(1 + errorDescription.size());

        payload[0] = errorCode;

        std::memcpy(&payload[1], errorDescription.data(), errorDescription.size());

        return payload;
    }

    /**
     * @brief Prepares the payload for a connection request (ConnectionRequest).
     * @param args The arguments required for the connection request (username).
     * @return A vector of bytes representing the connection request payload.
     */
    static std::vector<uint8_t> prepareConnectionRequest(
        const std::vector<std::string>& args
    ) {
        if (args.empty()) {
            std::cerr << "Invalid arguments for Connection Request!" << std::endl;
            return {};
        }

        std::string username = args[0];
        std::vector<uint8_t> payload(username.begin(), username.end());  // Conversion du nom d'utilisateur en vecteur d'octets

        return payload;
    }

    /**
     * @brief Prepares the payload for a player movement message (PlayerMovement).
     * @param args The arguments required for the player movement (Player ID, X, Y, Z coordinates).
     * @return A vector of bytes representing the player movement payload.
     */
    static std::vector<uint8_t> preparePlayerMovement(
        const std::vector<std::string>& args
    ) {
        if (args.size() < 4) {
            std::cerr << "Invalid arguments for Player Movement!" << std::endl;
            return {};
        }

        std::vector<uint8_t> payload;

        const uint32_t playerId = htonl(std::stoi(args[0]));
        const float x = std::stof(args[1]);
        const float y = std::stof(args[2]);
        const float z = std::stof(args[3]);

        // Conversion des floats en format réseau
        uint32_t x_net, y_net, z_net;
        std::memcpy(&x_net, &x, 4);
        x_net = htonl(x_net);
        std::memcpy(&y_net, &y, 4);
        y_net = htonl(y_net);
        std::memcpy(&z_net, &z, 4);
        z_net = htonl(z_net);

        // Ajout des données au payload
        payload.resize(16);
        std::memcpy(&payload[0], &playerId, 4);
        std::memcpy(&payload[4], &x_net, 4);
        std::memcpy(&payload[8], &y_net, 4);
        std::memcpy(&payload[12], &z_net, 4);

        return payload;
    }

    /**
     * @brief Prepares the payload for a player shoot message (PlayerShoot).
     * @param args The arguments required for the player shoot (Player ID, direction, weapon type).
     * @return A vector of bytes representing the player shoot payload.
     */
    static std::vector<uint8_t> preparePlayerShoot(
        const std::vector<std::string>& args
    ) {
        if (args.size() < 3) {
            std::cerr << "Invalid arguments for Player Shoot!" << std::endl;
            return {};
        }

        std::vector<uint8_t> payload(6); // Player ID (4) + Direction (1) + Weapon type (1)

        const uint32_t playerId = htonl(std::stoi(args[0]));
        const auto direction = static_cast<uint8_t>(std::stoi(args[1]));
        const auto weaponType = static_cast<uint8_t>(std::stoi(args[2]));

        std::memcpy(&payload[0], &playerId, 4);
        payload[4] = direction;
        payload[5] = weaponType;

        return payload;
    }

    /**
     * @brief Prepares the payload for a chat message (ChatMessage).
     * @param args The arguments required for the chat message (Player ID, message).
     * @return A vector of bytes representing the chat message payload.
     */
    static std::vector<uint8_t> prepareChatMessage(
        const std::vector<std::string>& args
    ) {
        if (args.size() < 2) {
            std::cerr << "Invalid arguments for Chat Message!" << std::endl;
            return {};
        }

        const uint32_t playerId = htonl(std::stoi(args[0]));
        const std::string& message = args[1];
        const auto messageLength = static_cast<uint16_t>(message.size());
        const uint16_t messageLengthNetworkOrder = htons(messageLength);

        std::vector<uint8_t> payload(6 + message.size());
        std::memcpy(&payload[0], &playerId, 4);
        std::memcpy(&payload[4], &messageLengthNetworkOrder, 2);
        std::memcpy(&payload[6], message.data(), message.size());

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

        if (length < 12) {
            std::cerr << "Received malformed packet: insufficient header size" << std::endl;
            this->sendRequest(client_endpoint, GDTPMessageType::ConnectionReject, { "Invalid packet" });
            return;
        }

        // Convertir le paquet en vector<uint8_t>
        std::vector packetData(packet.begin(), packet.begin() + length);

        // Lecture du header
        GDTPHeader header = GDTPHeader::fromBuffer(packetData);

        std::cout << "Packet details - Version: " << static_cast<int>(header.version)
                  << ", Message Type: " << static_cast<int>(header.messageType)
                  << ", Packet ID: " << header.packetId
                  << ", Payload Size: " << header.payloadSize << std::endl;

        if (static_cast<uint16_t>(length) < 12 + header.payloadSize) {
            std::cerr << "Received malformed packet: incorrect payload size" << std::endl;
            return;
        }

        // Extraction du payload
        const std::vector payload(packetData.begin() + 12, packetData.begin() + 12 + header.payloadSize);

        // Traiter le message selon le type
        processMessage(static_cast<GDTPMessageType>(header.messageType), payload, header, client_endpoint);
    }

    /**
     * @brief Processes the received message based on its type.
     * @param messageType The type of the received GDTP message.
     * @param payload The payload of the message.
     * @param header The header of the received message.
     * @param client_endpoint The endpoint of the client that sent the message.
     */
    void processMessage(
        GDTPMessageType messageType,
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
        // 1. Préparer le header et le payload de la réponse Connection Accept
        GDTPHeader header{};
        header.version = 0x01; // Version du protocole
        header.messageType = static_cast<uint8_t>(GDTPMessageType::ConnectionAccept); // Type de message
        header.packetId = std::chrono::system_clock::now().time_since_epoch().count(); // Utilisation d'un timestamp comme ID
        header.payloadSize = 0; // Pas de payload pour Connection Accept

        // 2. Convertir le header en buffer
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
