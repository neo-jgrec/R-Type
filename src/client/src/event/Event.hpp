/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Event
*/

#pragma once

#ifndef EVENT_HPP
#define EVENT_HPP

    #include <cstdint>
    #include <string>
    #include <variant>

    #include "../../../core/network/includes/RequestType.hpp"

/**
 * @struct PlayerMovement
 * @brief Represents the data for a player movement event.
 */
struct PlayerMovement {
    uint32_t playerId;
    float x;
    float y;
    float z;

    bool operator==(const PlayerMovement& other) const {
        return playerId == other.playerId && x == other.x && y == other.y && z == other.z;
    }
};

/**
 * @struct ChatMessage
 * @brief Represents the data for a chat message event.
 */
struct ChatMessage {
    uint32_t playerId;
    std::string message;

    bool operator==(const ChatMessage& other) const {
        return playerId == other.playerId && message == other.message;
    }
};

/**
 * @struct PlayerShoot
 * @brief Represents the data for a player shooting event.
 */
struct PlayerShoot {
    uint32_t playerId;
    uint8_t direction;
    uint8_t weaponType;

    bool operator==(const PlayerShoot& other) const {
        return playerId == other.playerId && direction == other.direction && weaponType == other.weaponType;
    }
};

/**
 * @struct NoData
 * @brief Represents an event that does not have a payload.
 */
struct NoData {
    operator bool() const {
        return true;
    }
};

/**
 * @struct PowerUpCollected
 * @brief Represents the data for a PowerUp collected event.
 */
struct PowerUpCollected {
    uint32_t playerId;
    uint32_t powerUpId;

    bool operator==(const PowerUpCollected &other) const
    {
        return this->playerId == other.playerId && this->powerUpId == other.powerUpId;
    }
};

/**
 * @struct EntityUpdate
 * @brief Represents the data for an entity update event.
 */
struct EntityUpdate {
    uint32_t entityId;
    float x;
    float y;
    float velocity;

    bool operator==(const EntityUpdate& other) const {
        return entityId == other.entityId && x == other.x && y == other.y && velocity == other.velocity;
    }
};

/**
 * @struct EntitySpawn
 * @brief Represents the data for an entity spawn event.
 */
struct EntitySpawn {
    uint32_t entityId;
    float x;
    float y;

    bool operator==(const EntitySpawn& other) const {
        return entityId == other.entityId && x == other.x && y == other.y;
    }
};

/**
 * @struct EntityDestroy
 * @brief Represents the data for an entity destroy event.
 */
struct EntityDestroy {
    uint32_t entityId;

    bool operator==(const EntityDestroy& other) const {
        return entityId == other.entityId;
    }
};

/**
 * @struct PlayerHealthUpdate
 * @brief Represents the data for a player health update event.
 */
struct PlayerHealthUpdate {
    uint32_t playerId;
    int health;

    bool operator==(const PlayerHealthUpdate &other) const
    {
        return  this->playerId == other.playerId && this->health == other.health;
    }
};
/**
 * @class Event
 * @brief Represents an event that can occur in the system, which could have different types of payloads.
 *
 * The Event class encapsulates the type of event (such as PlayerMovement, ChatMessage, etc.)
 * and the corresponding payload, if applicable.
 */
class Event {
public:
    /**
 * @brief Alias for GDTPMessageType. EventType now inherits all values from GDTPMessageType.
 */
    using EventType = GDTPMessageType;

    /**
     * @brief Constructor for an event with a PlayerMovement payload.
     * @param type The type of the event.
     * @param movement The PlayerMovement payload.
     */
    Event(EventType type, const PlayerMovement& movement);

    /**
     * @brief Constructor for an event with a ChatMessage payload.
     * @param type The type of the event.
     * @param message The ChatMessage payload.
     */
    Event(EventType type, const ChatMessage& message);

    /**
     * @brief Constructor for an event with a PlayerShoot payload.
     * @param type The type of the event.
     * @param shoot The PlayerShoot payload.
     */
    Event(EventType type, const PlayerShoot& shoot);

    /**
     * @brief Constructor for an event with a PowerUpCollected payload.
     * @param type The type of the event.
     * @param powerUp The PowerUpCollected payload.
     */
    Event(EventType type, const PowerUpCollected& powerUp);

    /**
     * @brief Constructor for an event with an EntityUpdate payload.
     * @param type The type of the event.
     * @param update The EntityUpdate payload.
     */
    Event(EventType type, const EntityUpdate& update);

    /**
     * @brief Constructor for an event with an EntitySpawn payload.
     * @param type The type of the event.
     * @param spawn The EntitySpawn payload.
     */
    Event(EventType type, const EntitySpawn& spawn);

    /**
     * @brief Constructor for an event with an EntityDestroy payload.
     * @param type The type of the event.
     * @param destroy The EntityDestroy payload.
     */
    Event(EventType type, const EntityDestroy& destroy);

    /**
 * @brief Constructor for an event with a PlayerHealthUpdate payload.
 * @param type The type of the event.
 * @param healthUpdate The PlayerHealthUpdate payload.
 */
    Event(EventType type, const PlayerHealthUpdate& healthUpdate);


    /**
     * @brief Constructor for an event without a payload.
     * @param type The type of the event.
     */
    Event(EventType type);

    /**
     * @brief Returns the type of the event.
     * @return The EventType of the event.
     */
    EventType getType() const;

    /**
     * @brief Returns the payload of the event.
     *
     * This function returns the payload, which could be one of the various types depending on the event type.
     * If no data exists, it returns `std::monostate`.
     *
     * @return A variant containing the event payload.
     */
    const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData>& getPayload() const;

    bool operator==(const Event& other) const {
        return this->type == other.type && this->payload == other.payload;
    }

private:
    EventType type; ///< The type of the event.
    std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData> payload; ///< The payload of the event.
};

#endif // EVENT_HPP
