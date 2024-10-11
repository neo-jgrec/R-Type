/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventFactory.cpp
*/

#include "EventFactory.hpp"
#include <iostream>
#include <cstring>

// Define the handlers map
const std::unordered_map<uint8_t, EventFactory::EventHandler> EventFactory::handlers = {
    {0x01, &EventFactory::handlePlayerMovement},
    {0x05, &EventFactory::handlePlayerShoot},
    {0x15, &EventFactory::handleChatMessage},
    {0x06, &EventFactory::handlePlayerHealthUpdate},
    {0x09, &EventFactory::handleEntitySpawn},
    {0x0A, &EventFactory::handleEntityDestroy},
};

Event EventFactory::createEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    uint8_t type = header.messageType;

    auto it = handlers.find(type);
    if (it != handlers.end()) {
        return it->second(header, payload);
    } else {
        throw EventPool::UnknownEvent(header.messageType);
    }
}

Event EventFactory::handlePlayerMovement([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerMovement)) {
        throw std::runtime_error("Invalid payload size for PlayerMovement");
    }

    PlayerMovement movement;
    std::memcpy(&movement.playerId, &payload[0], 4);
    std::memcpy(&movement.x, &payload[4], 4);
    std::memcpy(&movement.y, &payload[8], 4);
    std::memcpy(&movement.z, &payload[12], 4);

    return Event(0x01, movement);
}

Event EventFactory::handlePlayerShoot([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerShoot)) {
        throw std::runtime_error("Invalid payload size for PlayerShoot");
    }

    PlayerShoot shoot;
    std::memcpy(&shoot.playerId, &payload[0], 4);
    shoot.direction = payload[4];
    shoot.weaponType = payload[5];

    return Event(0x05, shoot);
}

Event EventFactory::handleChatMessage([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < 6) {
        throw std::runtime_error("Invalid payload size for ChatMessage");
    }

    uint32_t playerId;
    uint16_t messageLength;
    std::memcpy(&playerId, &payload[0], 4);
    std::memcpy(&messageLength, &payload[4], 2);

    std::string message(payload.begin() + 6, payload.begin() + 6 + messageLength);
    ChatMessage chatMessage{playerId, message};

    return Event(0x15, chatMessage);
}

Event EventFactory::handlePlayerHealthUpdate([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerHealthUpdate)) {
        throw std::runtime_error("Invalid payload size for PlayerHealthUpdate");
    }

    PlayerHealthUpdate healthUpdate;
    std::memcpy(&healthUpdate.playerId, &payload[0], 4);
    std::memcpy(&healthUpdate.health, &payload[4], 4);

    return Event(0x06, healthUpdate);
}

Event EventFactory::handleEntitySpawn([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(EntitySpawn)) {
        throw std::runtime_error("Invalid payload size for EntitySpawn");
    }

    EntitySpawn entitySpawn;
    std::memcpy(&entitySpawn.entityId, &payload[0], 4);
    std::memcpy(&entitySpawn.x, &payload[4], 4);
    std::memcpy(&entitySpawn.y, &payload[8], 4);

    return Event(0x09, entitySpawn);
}

Event EventFactory::handleEntityDestroy([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(EntityDestroy)) {
        throw std::runtime_error("Invalid payload size for EntityDestroy");
    }

    EntityDestroy entityDestroy;
    std::memcpy(&entityDestroy.entityId, &payload[0], 4);

    return Event(0x0A, entityDestroy);
}
