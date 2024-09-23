/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RequestHeader
*/

#pragma once

#ifndef REQUESTHEADER_HPP
    #define REQUESTHEADER_HPP
    #include <cstdint> // Pour les types uint8_t, uint16_t, uint64_t
    #include <vector>
    #include <cstring> // Pour memcpy
    #include <arpa/inet.h> // Pour htonl, htons, ntohl, ntohs

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

#endif //REQUESTHEADER_HPP
