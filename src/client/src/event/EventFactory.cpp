/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventFactory.cpp
*/

#include "EventFactory.hpp"
#include <iostream>
#include <cstring>

/**
 * @brief Creates an Event object from the provided GDTPHeader and payload.
 *
 * @param header The GDTPHeader containing event metadata.
 * @param payload The raw binary data of the event.
 * @return A fully constructed Event object.
 * @throws UnknownEvent if the event type is unsupported.
 */
Event EventFactory::createEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    Event::EventType type = uint8ToEventType(header.messageType);

    switch (type) {
        case Event::EventType::PlayerMovement:
            return handlePlayerMovement(header, payload);
        case Event::EventType::PlayerShoot:
            return handlePlayerShoot(header, payload);
        case Event::EventType::ChatMessage:
            return handleChatMessage(header, payload);
        case Event::EventType::PlayerHealthUpdate:
            return handlePlayerHealthUpdate(header, payload);
        case Event::EventType::EntitySpawn:
            return handleEntitySpawn(header, payload);
        case Event::EventType::EntityDestroy:
            return handleEntityDestroy(header, payload);
        default:
            throw EventPool::UnknownEvent(header.messageType);
    }
}

Event EventFactory::handlePlayerMovement(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerMovement)) {
        throw std::runtime_error("Invalid payload size for PlayerMovement");
    }

    PlayerMovement movement;
    std::memcpy(&movement.playerId, &payload[0], 4);
    std::memcpy(&movement.x, &payload[4], 4);
    std::memcpy(&movement.y, &payload[8], 4);
    std::memcpy(&movement.z, &payload[12], 4);

    return Event(Event::EventType::PlayerMovement, movement);
}

Event EventFactory::handlePlayerShoot(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerShoot)) {
        throw std::runtime_error("Invalid payload size for PlayerShoot");
    }

    PlayerShoot shoot;
    std::memcpy(&shoot.playerId, &payload[0], 4);
    shoot.direction = payload[4];
    shoot.weaponType = payload[5];

    return Event(Event::EventType::PlayerShoot, shoot);
}

Event EventFactory::handleChatMessage(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < 6) {
        throw std::runtime_error("Invalid payload size for ChatMessage");
    }

    uint32_t playerId;
    uint16_t messageLength;
    std::memcpy(&playerId, &payload[0], 4);
    std::memcpy(&messageLength, &payload[4], 2);

    std::string message(payload.begin() + 6, payload.begin() + 6 + messageLength);

    ChatMessage chatMessage{playerId, message};

    return Event(Event::EventType::ChatMessage, chatMessage);
}

Event EventFactory::handlePlayerHealthUpdate(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerHealthUpdate)) {
        throw std::runtime_error("Invalid payload size for PlayerHealthUpdate");
    }

    PlayerHealthUpdate healthUpdate;
    std::memcpy(&healthUpdate.playerId, &payload[0], 4);
    std::memcpy(&healthUpdate.health, &payload[4], 4);

    return Event(Event::EventType::PlayerHealthUpdate, healthUpdate);
}

Event EventFactory::handleEntitySpawn(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(EntitySpawn)) {
        throw std::runtime_error("Invalid payload size for EntitySpawn");
    }

    EntitySpawn entitySpawn;
    std::memcpy(&entitySpawn.entityId, &payload[0], 4);
    std::memcpy(&entitySpawn.x, &payload[4], 4);
    std::memcpy(&entitySpawn.y, &payload[8], 4);

    return Event(Event::EventType::EntitySpawn, entitySpawn);
}

Event EventFactory::handleEntityDestroy(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(EntityDestroy)) {
        throw std::runtime_error("Invalid payload size for EntityDestroy");
    }

    EntityDestroy entityDestroy;
    std::memcpy(&entityDestroy.entityId, &payload[0], 4);

    return Event(Event::EventType::EntityDestroy, entityDestroy);
}
