/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PreparePayload
*/

#pragma once

#ifndef PREPAREPAYLOAD_HPP
    #define PREPAREPAYLOAD_HPP
    #include <iostream>
    #include <vector>
    #include <cstring>
    #include <map>
    #include "../core/network/includes/RequestType.hpp"

namespace Payload {
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
     * @brief Returns a map of payload preparation functions for a few GDTP message types. (is the default implementation)
     * @return A shared pointer to the map of payload preparation functions.
     */
    static std::shared_ptr<std::map<uint8_t, std::function<std::vector<uint8_t>(std::vector<std::string>)>>> payloadMap()
    {

        auto payloadMap = std::make_shared<std::map<uint8_t, std::function<std::vector<uint8_t>(std::vector<std::string>)>>>();

        *payloadMap = {
        {+GDTPMessageType::ErrorMessage, prepareErrorMessage},
        {+GDTPMessageType::ConnectionRequest, prepareConnectionRequest},
        {+GDTPMessageType::PlayerMovement, preparePlayerMovement},
        {+GDTPMessageType::PlayerShoot, preparePlayerShoot},
        {+GDTPMessageType::ChatMessage, prepareChatMessage}
        };
        return payloadMap;
    };
}
#endif //PREPAREPAYLOAD_HPP
