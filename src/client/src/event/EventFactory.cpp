/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventFactory.cpp
*/

#include "EventFactory.hpp"
#include <cstring>
#include "../../../game/RequestType.hpp"
#include "src/event/Event.hpp"

// Define the handlers map
const std::unordered_map<uint8_t, EventFactory::EventHandler> EventFactory::handlers = {
    {RequestType::EnemySpawn, &EventFactory::handlePlayerMovement},
    {RequestType::PlayerShoot, &EventFactory::handlePlayerShoot},
    // {, &EventFactory::handleChatMessage},
    {RequestType::PlayerHit, &EventFactory::handlePlayerHealthUpdate},
    {RequestType::EnemySpawn, &EventFactory::handleEntitySpawn},
    {RequestType::EnemyDie, &EventFactory::handleEntityDestroy},
    {RequestType::MapScroll, &EventFactory::handleMapScroll},
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

Event EventFactory::handleMapScroll([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(MapScroll)) {
        throw std::runtime_error("Invalid payload size for MapScroll");
    }

    int mapScroll;
    mapScroll = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];

    return {0x0B, mapScroll};
}

Event EventFactory::handlePlayerMovement([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerMovement)) {
        throw std::runtime_error("Invalid payload size for PlayerMovement");
    }

    PlayerMovement movement{};
    movement.playerId = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    movement.x = static_cast<float>((payload[4] << 24) | (payload[5] << 16) | (payload[6] << 8) | payload[7]);
    movement.y = static_cast<float>((payload[8] << 24) | (payload[9] << 16) | (payload[10] << 8) | payload[11]);
    movement.z = static_cast<float>((payload[12] << 24) | (payload[13] << 16) | (payload[14] << 8) | payload[15]);

    return {0x01, movement};
}

Event EventFactory::handlePlayerShoot([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerShoot)) {
        throw std::runtime_error("Invalid payload size for PlayerShoot");
    }

    struct PlayerShoot shoot{};
    shoot.playerId = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    shoot.direction = payload[4];
    shoot.weaponType = payload[5];

    return {0x05, shoot};
}

Event EventFactory::handleChatMessage([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < 6) {
        throw std::runtime_error("Invalid payload size for ChatMessage");
    }

    uint32_t playerId;
    uint16_t messageLength;
    playerId = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    messageLength = (payload[4] << 8) | payload[5];

    std::string message(payload.begin() + 6, payload.begin() + 6 + messageLength);
    ChatMessage chatMessage{playerId, message};

    return {0x15, chatMessage};
}

Event EventFactory::handlePlayerHealthUpdate([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerHealthUpdate)) {
        throw std::runtime_error("Invalid payload size for PlayerHealthUpdate");
    }

    PlayerHealthUpdate healthUpdate{};
    healthUpdate.playerId = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    healthUpdate.health = (payload[4] << 24) | (payload[5] << 16) | (payload[6] << 8) | payload[7];

    return {0x06, healthUpdate};
}

Event EventFactory::handleEntitySpawn([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(EntitySpawn)) {
        throw std::runtime_error("Invalid payload size for EntitySpawn");
    }

    EntitySpawn entitySpawn{};
    entitySpawn.entityId = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    entitySpawn.x = static_cast<float>((payload[4] << 24) | (payload[5] << 16) | (payload[6] << 8) | payload[7]);
    entitySpawn.y = static_cast<float>((payload[8] << 24) | (payload[9] << 16) | (payload[10] << 8) | payload[11]);

    return {0x09, entitySpawn};
}

Event EventFactory::handleEntityDestroy([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(EntityDestroy)) {
        throw std::runtime_error("Invalid payload size for EntityDestroy");
    }

    EntityDestroy entityDestroy{};
    entityDestroy.entityId = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];

    return {0x0A, entityDestroy};
}
