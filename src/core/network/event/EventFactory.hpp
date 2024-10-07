/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventFactory.hpp
*/

#pragma once

#include <map>
#include <functional>
#include <stdexcept>
#include "Event.hpp" // Inclure la définition de la classe Event
#include "../includes/RequestHeader.hpp" // Inclure la définition de GDTPHeader

class EventFactory {
public:
    using EventCreatorFunc = std::function<Event(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>;

    // Création d'une map statique pour la gestion des fonctions
    static std::map<Event::EventType, EventCreatorFunc>& getEventCreators() {
        static std::map<Event::EventType, EventCreatorFunc> eventCreators = {
            { Event::EventType::PlayerMovement, createPlayerMovementEvent },
            { Event::EventType::PlayerShoot, createPlayerShootEvent },
            { Event::EventType::ChatMessage, createChatMessageEvent },
            { Event::EventType::PlayerHealthUpdate, createPlayerHealthUpdateEvent },
            { Event::EventType::EntitySpawn, createEntitySpawnEvent },
            { Event::EventType::EntityDestroy, createEntityDestroyEvent },
            { Event::EventType::PowerUpCollected, createPowerUpCollectedEvent },
            { Event::EventType::ConnectionRequest, createPayloadStringEvent },
            { Event::EventType::ConnectionAccept, createPayloadIDEvent }
        };
        return eventCreators;
    }

    // Fonction principale de création d'un événement
    static Event createEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        Event::EventType type = static_cast<Event::EventType>(header.messageType);

        // Récupérer le créateur de l'événement correspondant
        const auto& creators = getEventCreators();
        auto it = creators.find(type);
        if (it != creators.end()) {
            return it->second(header, payload, endpoint);
        }

        throw std::invalid_argument("Unknown event type");
    }

private:
    // Les fonctions de création d'événements existantes
    static Event createPlayerMovementEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        if (payload.size() < sizeof(PlayerMovement)) {
            throw std::invalid_argument("Invalid payload size for PlayerMovement");
        }

        PlayerMovement movement;
        std::memcpy(&movement.playerId, &payload[0], 4);
        std::memcpy(&movement.x, &payload[4], 4);
        std::memcpy(&movement.y, &payload[8], 4);
        std::memcpy(&movement.z, &payload[12], 4);
        return Event(Event::EventType::PlayerMovement, movement, header, endpoint);
    }

    static Event createPlayerShootEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        if (payload.size() < sizeof(PlayerShoot)) {
            throw std::invalid_argument("Invalid payload size for PlayerShoot");
        }

        PlayerShoot shoot;
        std::memcpy(&shoot.playerId, &payload[0], 4);
        shoot.direction = payload[4];
        shoot.weaponType = payload[5];
        return Event(Event::EventType::PlayerShoot, shoot, header, endpoint);
    }

    static Event createChatMessageEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        if (payload.size() < 6) {
            throw std::invalid_argument("Invalid payload size for ChatMessage");
        }

        uint32_t playerId;
        uint16_t messageLength;
        std::memcpy(&playerId, &payload[0], 4);
        std::memcpy(&messageLength, &payload[4], 2);

        std::string message(payload.begin() + 6, payload.begin() + 6 + messageLength);
        return Event(static_cast<Event::EventType>(header.messageType), ChatMessage{ playerId, message }, header, endpoint);
    }

    static Event createPlayerHealthUpdateEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        if (payload.size() < sizeof(PlayerHealthUpdate)) {
            throw std::invalid_argument("Invalid payload size for PlayerHealthUpdate");
        }

        PlayerHealthUpdate healthUpdate;
        std::memcpy(&healthUpdate.playerId, &payload[0], 4);
        std::memcpy(&healthUpdate.health, &payload[4], 4);
        return Event(static_cast<Event::EventType>(header.messageType), healthUpdate, header, endpoint);
    }

    static Event createEntitySpawnEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        if (payload.size() < sizeof(EntitySpawn)) {
            throw std::invalid_argument("Invalid payload size for EntitySpawn");
        }

        EntitySpawn spawn;
        std::memcpy(&spawn.entityId, &payload[0], 4);
        std::memcpy(&spawn.x, &payload[4], 4);
        std::memcpy(&spawn.y, &payload[8], 4);
        return Event(static_cast<Event::EventType>(header.messageType), spawn, header, endpoint);
    }

    static Event createEntityDestroyEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        if (payload.size() < sizeof(EntityDestroy)) {
            throw std::invalid_argument("Invalid payload size for EntityDestroy");
        }

        EntityDestroy destroy;
        std::memcpy(&destroy.entityId, &payload[0], 4);
        return Event(static_cast<Event::EventType>(header.messageType), destroy, header, endpoint);
    }

    static Event createPowerUpCollectedEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        if (payload.size() < sizeof(PowerUpCollected)) {
            throw std::invalid_argument("Invalid payload size for PowerUpCollected");
        }

        PowerUpCollected powerUp;
        std::memcpy(&powerUp.playerId, &payload[0], 4);
        powerUp.powerUpId = payload[4];
        return Event(static_cast<Event::EventType>(header.messageType), powerUp, header, endpoint);
    }

    static Event createPayloadStringEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        if (payload.empty()) {
            throw std::invalid_argument("Payload is empty for payloadString");
        }

        std::string message(payload.begin(), payload.end());
        return Event(static_cast<Event::EventType>(header.messageType), header, message, endpoint);
    }

    static Event createPayloadIDEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        if (payload.size() < sizeof(int)) {
            throw std::invalid_argument("Invalid payload size for payloadID");
        }

        int id;
        std::memcpy(&id, &payload[0], sizeof(int));
        return Event(static_cast<Event::EventType>(header.messageType), header, id, endpoint);
    }
};



