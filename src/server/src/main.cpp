#include <iostream>
#include "../core/network/NetworkService.hpp"
#include <thread>
#include <fstream>

std::vector<uint8_t> generatePlayerMovementPacket(uint32_t playerId, float x, float y, float z) {
    std::vector<uint8_t> packet;

    // 1. Header (10 octets)
    uint8_t version = 0x01;  // Version du protocole
    GDTPMessageType messageType = GDTPMessageType::PlayerMovement;
    uint64_t packetId = std::chrono::system_clock::now().time_since_epoch().count();  // Utilisation du timestamp pour le Packet ID
    uint16_t payloadSize = 16;  // Taille du payload (Player ID (4) + X, Y, Z (4, 4, 4))

    // Insertion des champs dans le header
    packet.push_back(version);  // Version
    packet.push_back(static_cast<uint8_t>(messageType));  // Message Type

    // Insertion du Packet ID (8 octets)
    uint8_t packetIdBytes[8];
    std::memcpy(packetIdBytes, &packetId, 8);
    packet.insert(packet.end(), packetIdBytes, packetIdBytes + 8);

    // Insertion de la taille du payload (2 octets)
    uint8_t payloadSizeBytes[2];
    std::memcpy(payloadSizeBytes, &payloadSize, 2);
    packet.insert(packet.end(), payloadSizeBytes, payloadSizeBytes + 2);

    // 2. Payload (16 octets)
    // Insertion du Player ID
    uint8_t playerIdBytes[4];
    std::memcpy(playerIdBytes, &playerId, 4);
    packet.insert(packet.end(), playerIdBytes, playerIdBytes + 4);

    // Insertion des coordonnées X, Y, Z (floats)
    uint8_t xBytes[4], yBytes[4], zBytes[4];
    std::memcpy(xBytes, &x, 4);
    std::memcpy(yBytes, &y, 4);
    std::memcpy(zBytes, &z, 4);
    packet.insert(packet.end(), xBytes, xBytes + 4);
    packet.insert(packet.end(), yBytes, yBytes + 4);
    packet.insert(packet.end(), zBytes, zBytes + 4);

    return packet;  // Paquet complet prêt à être envoyé
}

int main() {
    // Serveur initialisé avec l'IP 0.0.0.0 (toutes les interfaces) et le port 12345
    NetworkingService serverService("0.0.0.0", 12345);

    // Lancement de l'écoute asynchrone dans un thread séparé
    // std::jthread serverThread([&serverService]() {
        serverService.run(); // Exécute le contexte Asio
    // });

    std::cout << "Server is running and listening on all interfaces (0.0.0.0:12345)" << std::endl;

    // Attendre que le serveur fonctionne indéfiniment
    // serverThread.join();

    serverService.attempt();
    return 0;
}


//int main() {
//    NetworkingService networkService("127.0.0.1", 12345);
//
//    // Envoi d'une requête de mouvement du joueur
//    std::vector<std::string> movementArgs = { "1", "100.0", "200.0", "300.0" };
//    networkService.sendRequest("127.0.0.1", GDTPMessageType::PlayerMovement, movementArgs);
//
//    // Envoi d'une requête de tir
//    std::vector<std::string> shootArgs = { "1", "2", "1" };
//    networkService.sendRequest("127.0.0.1", GDTPMessageType::PlayerShoot, shootArgs);
//
//    return 0;
//}
