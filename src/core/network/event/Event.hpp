/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Event
*/

#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <asio/ip/udp.hpp>
#include "../core/network/includes/RequestHeader.hpp"  // Inclure le header pour GDTPHeader
#include "../includes/RequestType.hpp"

struct PlayerMovement {
    uint32_t playerId;
    float x;
    float y;
    float z;
};

struct ChatMessage {
    uint32_t playerId;
    std::string message;
};

struct PlayerShoot {
    uint32_t playerId;
    uint8_t direction;
    uint8_t weaponType;
};

struct PlayerHealthUpdate {
    uint32_t playerId;
    int health;
};

struct EntitySpawn {
    uint32_t entityId;
    float x;
    float y;
};

struct EntityDestroy {
    uint32_t entityId;
};

struct PowerUpCollected {
    uint32_t playerId;
    uint32_t powerUpId;
};

struct NoData {};

class Event {
public:
    typedef GDTPMessageType EventType;

    Event(EventType type, const PlayerMovement& movement, const GDTPHeader& header, const asio::ip::udp::endpoint& endpoint)
        : type(type), payload(movement), header(header), clientEndpoint(endpoint) {}

    Event(EventType type, const ChatMessage& message, const GDTPHeader& header, const asio::ip::udp::endpoint& endpoint)
        : type(type), payload(message), header(header), clientEndpoint(endpoint) {}

    Event(EventType type, const PlayerShoot& shoot, const GDTPHeader& header, const asio::ip::udp::endpoint& endpoint)
        : type(type), payload(shoot), header(header), clientEndpoint(endpoint) {}

    Event(EventType type, const PlayerHealthUpdate& healthUpdate, const GDTPHeader& header, const asio::ip::udp::endpoint& endpoint)
        : type(type), payload(healthUpdate), header(header), clientEndpoint(endpoint) {}

    Event(EventType type, const EntitySpawn& spawn, const GDTPHeader& header, const asio::ip::udp::endpoint& endpoint)
        : type(type), payload(spawn), header(header), clientEndpoint(endpoint) {}

    Event(EventType type, const EntityDestroy& destroy, const GDTPHeader& header, const asio::ip::udp::endpoint& endpoint)
        : type(type), payload(destroy), header(header), clientEndpoint(endpoint) {}

    Event(EventType type, const PowerUpCollected& powerUp, const GDTPHeader& header, const asio::ip::udp::endpoint& endpoint)
        : type(type), payload(powerUp), header(header), clientEndpoint(endpoint) {}

    Event(EventType type, const GDTPHeader& header, const asio::ip::udp::endpoint& endpoint)
        : type(type), payload(NoData()), header(header), clientEndpoint(endpoint) {}

    Event(EventType type, const GDTPHeader& header, const std::string &payload, const asio::ip::udp::endpoint& endpoint)
    : type(type), payload(payload), header(header), clientEndpoint(endpoint) {}

    Event(EventType type, const GDTPHeader& header, int payload, const asio::ip::udp::endpoint& endpoint)
    : type(type), payload(payload), header(header), clientEndpoint(endpoint) {}

    EventType getType() const {
        return type;
    }

    const GDTPHeader& getHeader() const {
        return header;
    }

    const asio::ip::udp::endpoint& getClientEndpoint() const {
        return clientEndpoint;
    }

    const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PlayerHealthUpdate, EntitySpawn, EntityDestroy, PowerUpCollected, NoData, std::string, int>& getPayload() const {
        return payload;
    }

private:
    EventType type;
    std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PlayerHealthUpdate, EntitySpawn, EntityDestroy, PowerUpCollected, NoData, std::string, int> payload;
    GDTPHeader header;  // Stocker le header
    asio::ip::udp::endpoint clientEndpoint;  // Stocker l'adresse de l'expéditeur
};



