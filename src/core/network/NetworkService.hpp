#pragma once

#ifndef NETWORKSERVICE_HPP
#define NETWORKSERVICE_HPP

    #include <asio.hpp>
    #include <chrono>
    #include <cstdint> // Pour les types uint8_t, uint16_t, uint64_t
    #include <cstring>
    #include <functional> // Pour gérer les callbacks
    #include <iostream>
    #include <string>
    #include <thread>
    #include <vector>

// Fonctions auxiliaires pour htonll et ntohll (pour les entiers 64 bits)
inline uint64_t htonll(uint64_t value)
{
    if (htonl(1) != 1) { // Vérifie l'ordre des octets
        return (static_cast<uint64_t>(htonl(value & 0xFFFFFFFF)) << 32) | htonl(value >> 32);
    }
    return value;
}

inline uint64_t ntohll(uint64_t value) {
    if (ntohl(1) != 1) {  // Vérifie l'ordre des octets
        return (static_cast<uint64_t>(ntohl(value & 0xFFFFFFFF)) << 32) | ntohl(value >> 32);
    }
    return value;
}

// Structure pour représenter un header GDTP
struct GDTPHeader {
    uint8_t version;         // Version du protocole (1 octet)
    uint8_t messageType;     // Type de message (1 octet)
    uint64_t packetId;       // Identifiant unique du paquet (8 octets)
    uint16_t payloadSize;    // Taille du payload (2 octets)

    // Convertit les éléments de la structure en un buffer binaire pour l'envoi
    std::vector<uint8_t> toBuffer() const {
        std::vector<uint8_t> buffer(12);  // 12 octets pour le header

        buffer[0] = version;
        buffer[1] = messageType;

        uint64_t packetIdNetworkOrder = htonll(packetId);  // Conversion en big-endian
        std::memcpy(&buffer[2], &packetIdNetworkOrder, 8); // Copie du packetId

        uint16_t payloadSizeNetworkOrder = htons(payloadSize);  // Conversion en big-endian
        std::memcpy(&buffer[10], &payloadSizeNetworkOrder, 2);  // Copie de la taille du payload

        return buffer;
    }

    // Remplit la structure à partir d'un buffer binaire reçu
    static GDTPHeader fromBuffer(const std::vector<uint8_t>& buffer) {
        GDTPHeader header;
        header.version = buffer[0];
        header.messageType = buffer[1];

        uint64_t packetIdNetworkOrder;
        std::memcpy(&packetIdNetworkOrder, &buffer[2], 8);  // Récupération du packetId
        header.packetId = ntohll(packetIdNetworkOrder);  // Conversion en format hôte (little-endian)

        uint16_t payloadSizeNetworkOrder;
        std::memcpy(&payloadSizeNetworkOrder, &buffer[10], 2);  // Récupération de la taille du payload
        header.payloadSize = ntohs(payloadSizeNetworkOrder);  // Conversion en format hôte

        return header;
    }
};

// Enum pour les types de message GDTP
enum class GDTPMessageType : uint8_t {
    PlayerMovement = 0x01,
    GameEvent = 0x02,
    EntityUpdate = 0x03,
    Acknowledgment = 0x04,
    PlayerShoot = 0x05,
    PlayerHealthUpdate = 0x06,
    PlayerRespawn = 0x07,
    PlayerDisconnect = 0x08,
    EntitySpawn = 0x09,
    EntityDestroy = 0x0A,
    PowerUpCollected = 0x0B,
    GameStart = 0x0C,
    GameOver = 0x0D,
    PingRequest = 0x0E,
    PingResponse = 0x0F,
    ConnectionRequest = 0x10,
    ConnectionAccept = 0x11,
    ConnectionReject = 0x12,
    Synchronization = 0x13,
    ErrorMessage = 0x14,
    ChatMessage = 0x15,
    GamePause = 0x16,
    MapData = 0x20,
    MapScroll = 0x21
};

class NetworkingService {
public:
    NetworkingService(const std::string& server_ip, int port)
        : socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
        std::cout << "Server listening on " << server_ip << ":" << port << std::endl;
        startReceive(); // Commence l'écoute des paquets dès l'initialisation
    }

    ~NetworkingService() {
        socket_.close();
        this->stop();
    }

    // Méthode pour envoyer une requête
    void sendRequest(const std::string& recipient, int port, GDTPMessageType messageType, const std::vector<std::string>& args = {}) {
        std::vector<uint8_t> payload = preparePayload(messageType, args);
        std::cout << "Payload size: " << payload.size() << std::endl;

        // Création du header en utilisant GDTPHeader
        GDTPHeader header;
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

    // Démarrer l'écoute des paquets entrants
    void startReceive() {
        socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            [this](std::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    handleReceivedPacket(recv_buffer_, bytes_recvd, remote_endpoint_);
                }
                startReceive(); // Réécoute pour le prochain paquet
            }
        );
    }

    void run() {
        this->thread = std::jthread([this]() {
            io_context_.run();
        });
    }
    void stop()
    {
        io_context_.stop();
        this->thread.request_stop();
        this->thread.join();
    }

    void attempt()
    {
        thread.join();
    }

private:
    asio::io_context io_context_;
    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint remote_endpoint_; // Endpoint distant (celui qui envoie les données)
    std::array<uint8_t, 1400> recv_buffer_;   // Tampon pour recevoir les paquets

    // Prépare le payload en fonction du type de message
    std::vector<uint8_t> preparePayload(GDTPMessageType messageType, const std::vector<std::string>& args) {
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
            // Ajout d'autres cas pour d'autres types de message
            default:
                std::cerr << "Message type not implemented!" << std::endl;
                break;
        }
        return payload;
    }

    std::vector<uint8_t> prepareConnectionRequest(const std::vector<std::string>& args) {
        if (args.size() < 1) {
            std::cerr << "Invalid arguments for Connection Request!" << std::endl;
            return {};
        }

        std::string username = args[0];
        std::vector<uint8_t> payload(username.begin(), username.end());  // Conversion du nom d'utilisateur en vecteur d'octets

        return payload;
    }

    // Exemple de préparation du payload pour le mouvement du joueur
    std::vector<uint8_t> preparePlayerMovement(const std::vector<std::string>& args) {
        if (args.size() < 4) {
            std::cerr << "Invalid arguments for Player Movement!" << std::endl;
            return {};
        }

        std::vector<uint8_t> payload;

        uint32_t playerId = htonl(std::stoi(args[0]));
        float x = std::stof(args[1]);
        float y = std::stof(args[2]);
        float z = std::stof(args[3]);

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

    // Exemple de préparation du payload pour le tir du joueur
    std::vector<uint8_t> preparePlayerShoot(const std::vector<std::string>& args) {
        if (args.size() < 3) {
            std::cerr << "Invalid arguments for Player Shoot!" << std::endl;
            return {};
        }

        std::vector<uint8_t> payload(6); // Player ID (4) + Direction (1) + Weapon type (1)

        uint32_t playerId = htonl(std::stoi(args[0]));
        uint8_t direction = static_cast<uint8_t>(std::stoi(args[1]));
        uint8_t weaponType = static_cast<uint8_t>(std::stoi(args[2]));

        std::memcpy(&payload[0], &playerId, 4);
        payload[4] = direction;
        payload[5] = weaponType;

        return payload;
    }

    // Exemple de préparation du payload pour un message de chat
    std::vector<uint8_t> prepareChatMessage(const std::vector<std::string>& args) {
        if (args.size() < 2) {
            std::cerr << "Invalid arguments for Chat Message!" << std::endl;
            return {};
        }

        uint32_t playerId = htonl(std::stoi(args[0]));
        std::string message = args[1];
        uint16_t messageLength = static_cast<uint16_t>(message.size());
        uint16_t messageLengthNetworkOrder = htons(messageLength);

        std::vector<uint8_t> payload(6 + message.size());
        std::memcpy(&payload[0], &playerId, 4);
        std::memcpy(&payload[4], &messageLengthNetworkOrder, 2);
        std::memcpy(&payload[6], message.data(), message.size());

        return payload;
    }

    // Envoi du paquet au client destinataire
    void sendPacket(const std::vector<uint8_t>& packet, const std::string& recipient, int port) {
        asio::ip::udp::endpoint recipient_endpoint(asio::ip::address::from_string(recipient), port);
        std::error_code ec; // Code d'erreur pour capturer l'état de l'envoi
        size_t bytesSent = socket_.send_to(asio::buffer(packet), recipient_endpoint, 0, ec);


        // Vérification du code d'erreur
        if (ec) {
            // En cas d'erreur, lever une exception avec un message descriptif
            throw std::runtime_error("Failed to send packet: " + ec.message());
        } else {
            std::cout << "Packet successfully sent to " << recipient
                      << " (" << bytesSent << " bytes)" << std::endl;
        }
    }

    // Traitement des paquets reçus
    void handleReceivedPacket(const std::array<uint8_t, 1400>& packet, std::size_t length, const asio::ip::udp::endpoint& client_endpoint) {
        std::cout << "Received packet of length: " << length << std::endl;

        if (length < 12) {
            std::cerr << "Received malformed packet: insufficient header size" << std::endl;
            return;
        }

        // Convertir le paquet en vector<uint8_t>
        std::vector<uint8_t> packetData(packet.begin(), packet.begin() + length);

        // Lecture du header
        GDTPHeader header = GDTPHeader::fromBuffer(packetData);

        std::cout << "Packet details - Version: " << (int)header.version
                  << ", Message Type: " << (int)header.messageType
                  << ", Packet ID: " << header.packetId
                  << ", Payload Size: " << header.payloadSize << std::endl;

        if (length < 12 + header.payloadSize) {
            std::cerr << "Received malformed packet: incorrect payload size" << std::endl;
            return;
        }

        // Extraction du payload
        std::vector<uint8_t> payload(packetData.begin() + 12, packetData.begin() + 12 + header.payloadSize);

        // Traiter le message selon le type
        processMessage(static_cast<GDTPMessageType>(header.messageType), payload, header, client_endpoint);
    }

    // Logique de traitement des messages reçus
    void processMessage(GDTPMessageType messageType, const std::vector<uint8_t>& payload, const GDTPHeader& header, const asio::ip::udp::endpoint& client_endpoint) {
        switch (messageType) {
            case GDTPMessageType::PlayerMovement:
                handlePlayerMovement(payload);
                break;
            case GDTPMessageType::PlayerShoot:
                handlePlayerShoot(payload);
                break;
            case GDTPMessageType::ChatMessage:
                handleChatMessage(payload);
                break;
            case GDTPMessageType::ConnectionRequest:
                handleConnectionRequest(payload, client_endpoint);
                break;
            case GDTPMessageType::ConnectionAccept:
                handleConnectionAccept(payload);
                break;
            default:
                std::cerr << "Received unsupported message type!" << std::endl;
                break;
        }
    }
    void handleConnectionRequest(const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint) {
        // 1. Vérifier si le payload contient un nom d'utilisateur
        // if (payload.empty()) {
        //     std::cerr << "Received Connection Request with an empty payload!" << std::endl;
        //     return;
        // }

        // 2. Convertir le payload en nom d'utilisateur (chaîne de caractères)
        std::string username(payload.begin(), payload.end());
        std::cout << "Received Connection Request from username: " << username << std::endl;

        // 3. Récupérer l'IP et le port du client
        std::string client_ip = client_endpoint.address().to_string();
        uint16_t client_port = client_endpoint.port();

        std::cout << "Client IP: " << client_ip << ", Client Port: " << client_port << std::endl;

        // 4. Envoyer une réponse de "Connection Accept" au client
        sendConnectionAccept(client_endpoint);
    }

    void sendConnectionAccept(const asio::ip::udp::endpoint& client_endpoint) {
        // 1. Préparer le header et le payload de la réponse Connection Accept
        GDTPHeader header;
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

    void handleConnectionAccept(const std::vector<uint8_t>& payload)
    {
        std::cout << "Received Connection Accept!" << std::endl;
    }


    // Exemple de traitement du message PlayerMovement
    void handlePlayerMovement(const std::vector<uint8_t>& payload) {
        if (payload.size() < 16) {
            std::cerr << "Invalid PlayerMovement payload!" << std::endl;
            return;
        }

        uint32_t playerId_net;
        uint32_t x_net, y_net, z_net;
        std::memcpy(&playerId_net, &payload[0], 4);
        std::memcpy(&x_net, &payload[4], 4);
        std::memcpy(&y_net, &payload[8], 4);
        std::memcpy(&z_net, &payload[12], 4);

        uint32_t playerId = ntohl(playerId_net);

        float x, y, z;
        x_net = ntohl(x_net);
        y_net = ntohl(y_net);
        z_net = ntohl(z_net);
        std::memcpy(&x, &x_net, 4);
        std::memcpy(&y, &y_net, 4);
        std::memcpy(&z, &z_net, 4);

        std::cout << "Player " << playerId << " moved to position (" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    // Exemple de traitement du message PlayerShoot
    void handlePlayerShoot(const std::vector<uint8_t>& payload) {
        if (payload.size() < 6) {
            std::cerr << "Invalid PlayerShoot payload!" << std::endl;
            return;
        }

        uint32_t playerId_net;
        std::memcpy(&playerId_net, &payload[0], 4);
        uint32_t playerId = ntohl(playerId_net);

        uint8_t direction = payload[4];
        uint8_t weaponType = payload[5];

        std::cout << "Player " << playerId << " shot in direction " << (int)direction << " with weapon " << (int)weaponType << std::endl;
    }

    // Exemple de traitement du message ChatMessage
    void handleChatMessage(const std::vector<uint8_t>& payload) {
        if (payload.size() < 6) {
            std::cerr << "Invalid ChatMessage payload!" << std::endl;
            return;
        }

        uint32_t playerId_net;
        uint16_t messageLength_net;
        std::memcpy(&playerId_net, &payload[0], 4);
        std::memcpy(&messageLength_net, &payload[4], 2);

        uint32_t playerId = ntohl(playerId_net);
        uint16_t messageLength = ntohs(messageLength_net);

        if (payload.size() < 6 + messageLength) {
            std::cerr << "Invalid ChatMessage payload: incorrect message length" << std::endl;
            return;
        }

        std::string message(payload.begin() + 6, payload.begin() + 6 + messageLength);
        std::cout << "Player " << playerId << " says: " << message << std::endl;
    }
    std::jthread thread;
};

#endif // NETWORKSERVICE_HPP
