/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Event
*/

#pragma once

#ifndef EVENT_HPP
#define EVENT_HPP

    #include <SFML/System/Vector2.hpp>
    #include <cstddef>
    #include <cstdint>
    #include <string>
    #include <variant>
    #include <ostream>

/**
 * @struct PlayerMovement
 * @brief Represents the data for a player movement event.
 */
struct PlayerMovement {
    uint32_t playerId; ///< The unique ID of the player.
    float x; ///< The X coordinate of the player's position.
    float y; ///< The Y coordinate of the player's position.
    float z; ///< The Z coordinate of the player's position.

    /**
     * @brief Equality operator to compare two PlayerMovement objects.
     * @param other The other PlayerMovement object to compare with.
     * @return True if the player IDs and positions are equal, otherwise false.
     */
    bool operator==(const PlayerMovement& other) const {
        return playerId == other.playerId && x == other.x && y == other.y && z == other.z;
    }
};

/**
 * @struct ChatMessage
 * @brief Represents the data for a chat message event.
 */
struct ChatMessage {
    uint32_t playerId; ///< The unique ID of the player sending the message.
    std::string message; ///< The message content sent by the player.

    /**
     * @brief Equality operator to compare two ChatMessage objects.
     * @param other The other ChatMessage object to compare with.
     * @return True if the player IDs and messages are equal, otherwise false.
     */
    bool operator==(const ChatMessage& other) const {
        return playerId == other.playerId && message == other.message;
    }
};

/**
 * @struct PlayerShoot
 * @brief Represents the data for a player shooting event.
 */
struct PlayerShoot {
    uint32_t playerId; ///< The unique ID of the player shooting.
    uint8_t direction; ///< The direction of the shot (e.g., up, down, left, right).
    uint8_t weaponType; ///< The type of weapon used for the shot.

    /**
     * @brief Equality operator to compare two PlayerShoot objects.
     * @param other The other PlayerShoot object to compare with.
     * @return True if the player IDs, directions, and weapon types are equal, otherwise false.
     */
    bool operator==(const PlayerShoot& other) const {
        return playerId == other.playerId && direction == other.direction && weaponType == other.weaponType;
    }
};

/**
 * @struct NoData
 * @brief Represents an event that does not have a payload.
 */
struct NoData {
    /**
     * @brief Conversion operator to bool, used for checking the presence of data.
     * @return Always returns true as it signifies the presence of an empty payload.
     */
    operator bool() const {
        return true;
    }
};

/**
 * @struct PowerUpCollected
 * @brief Represents the data for a PowerUp collected event.
 */
struct PowerUpCollected {
    uint32_t playerId; ///< The unique ID of the player who collected the power-up.
    uint32_t powerUpId; ///< The ID of the collected power-up.

    /**
     * @brief Equality operator to compare two PowerUpCollected objects.
     * @param other The other PowerUpCollected object to compare with.
     * @return True if the player IDs and power-up IDs are equal, otherwise false.
     */
    bool operator==(const PowerUpCollected &other) const {
        return this->playerId == other.playerId && this->powerUpId == other.powerUpId;
    }
};

/**
 * @struct EntityUpdate
 * @brief Represents the data for an entity update event.
 */
struct EntityUpdate {
    uint32_t entityId; ///< The unique ID of the entity being updated.
    float x; ///< The X coordinate of the entity's position.
    float y; ///< The Y coordinate of the entity's position.
    float velocity; ///< The velocity of the entity.

    /**
     * @brief Equality operator to compare two EntityUpdate objects.
     * @param other The other EntityUpdate object to compare with.
     * @return True if the entity IDs, positions, and velocities are equal, otherwise false.
     */
    bool operator==(const EntityUpdate& other) const {
        return entityId == other.entityId && x == other.x && y == other.y && velocity == other.velocity;
    }
};

/**
 * @struct EntitySpawn
 * @brief Represents the data for an entity spawn event.
 */
struct EntitySpawn {
    uint32_t entityId; ///< The unique ID of the entity being spawned.
    float x; ///< The X coordinate of the entity's spawn position.
    float y; ///< The Y coordinate of the entity's spawn position.

    /**
     * @brief Equality operator to compare two EntitySpawn objects.
     * @param other The other EntitySpawn object to compare with.
     * @return True if the entity IDs and spawn positions are equal, otherwise false.
     */
    bool operator==(const EntitySpawn& other) const {
        return entityId == other.entityId && x == other.x && y == other.y;
    }
};

/**
 * @struct EntityDestroy
 * @brief Represents the data for an entity destroy event.
 */
struct EntityDestroy {
    uint32_t entityId; ///< The unique ID of the entity being destroyed.

    /**
     * @brief Equality operator to compare two EntityDestroy objects.
     * @param other The other EntityDestroy object to compare with.
     * @return True if the entity IDs are equal, otherwise false.
     */
    bool operator==(const EntityDestroy& other) const {
        return entityId == other.entityId;
    }
};

/**
 * @struct PlayerHealthUpdate
 * @brief Represents the data for a player health update event.
 */
struct PlayerHealthUpdate {
    uint32_t playerId; ///< The unique ID of the player.
    int health; ///< The updated health value of the player.

    /**
     * @brief Equality operator to compare two PlayerHealthUpdate objects.
     * @param other The other PlayerHealthUpdate object to compare with.
     * @return True if the player IDs and health values are equal, otherwise false.
     */
    bool operator==(const PlayerHealthUpdate &other) const {
        return this->playerId == other.playerId && this->health == other.health;
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
     * @brief Constructor for an event without a payload.
     * @param type The type of the event.
     */
    Event(uint8_t type, std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData, int, std::string, sf::Vector2f, std::pair<size_t, sf::Vector2f>> payload = std::monostate{});

    /**
     * @brief Returns the type of the event.
     * @return The EventType of the event.
     */
    [[nodiscard]] uint8_t getType() const;

    /**
     * @brief Returns the payload of the event.
     *
     * This function returns the payload, which could be one of the various types depending on the event type.
     * If no data exists, it returns `std::monostate`.
     *
     * @return A variant containing the event payload.
     */
    [[nodiscard]] const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData, int, std::string, sf::Vector2f, std::pair<size_t, sf::Vector2f>>& getPayload() const;

    /**
     * @brief Equality operator to compare two Event objects.
     * @param other The other Event object to compare with.
     * @return True if the event types and payloads are equal, otherwise false.
     */
    bool operator==(const Event& other) const {
        return this->type == other.type && this->payload == other.payload;
    }

    friend std::ostream& operator<<(std::ostream& os, const Event& event) {
        os << "Event{type=" << static_cast<int>(event.type) << ", payload=";
        std::visit([&os](const auto& payload) {
            using T = std::decay_t<decltype(payload)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                os << "NoData";
            } else if constexpr (std::is_same_v<T, PlayerMovement>) {
                os << "PlayerMovement{playerId=" << payload.playerId << ", x=" << payload.x << ", y=" << payload.y << ", z=" << payload.z << "}";
            } else if constexpr (std::is_same_v<T, ChatMessage>) {
                os << "ChatMessage{playerId=" << payload.playerId << ", message=" << payload.message << "}";
            } else if constexpr (std::is_same_v<T, PlayerShoot>) {
                os << "PlayerShoot{playerId=" << payload.playerId << ", direction=" << static_cast<int>(payload.direction) << ", weaponType=" << static_cast<int>(payload.weaponType) << "}";
            } else if constexpr (std::is_same_v<T, PowerUpCollected>) {
                os << "PowerUpCollected{playerId=" << payload.playerId << ", powerUpId=" << payload.powerUpId << "}";
            } else if constexpr (std::is_same_v<T, EntityUpdate>) {
                os << "EntityUpdate{entityId=" << payload.entityId << ", x=" << payload.x << ", y=" << payload.y << ", velocity=" << payload.velocity << "}";
            } else if constexpr (std::is_same_v<T, PlayerHealthUpdate>) {
                os << "PlayerHealthUpdate{playerId=" << payload.playerId << ", health=" << payload.health << "}";
            } else if constexpr (std::is_same_v<T, EntitySpawn>) {
                os << "EntitySpawn{entityId=" << payload.entityId << ", x=" << payload.x << ", y=" << payload.y << "}";
            } else if constexpr (std::is_same_v<T, EntityDestroy>) {
                os << "EntityDestroy{entityId=" << payload.entityId << "}";
            } else if constexpr (std::is_same_v<T, int>) {
                os << "int{" << payload << "}";
            } else if constexpr (std::is_same_v<T, NoData>) {
                os << "NoData{}";
            } else if constexpr (std::is_same_v<T, std::string>) {
                os << "string{" << payload << "}";
            } else if constexpr (std::is_same_v<T, sf::Vector2f>) {
                os << "Vector2f{x=" << payload.x << ", y=" << payload.y << "}";
            } else if constexpr (std::is_same_v<T, std::pair<size_t, sf::Vector2f>>) {
                os << "pair{first=" << payload.first << ", second={x=" << payload.second.x << ", y=" << payload.second.y << "}}";
            } else {
                static_assert(always_false<T>::value, "non-exhaustive visitor!");
            }
        }, event.payload);
        os << "}";
        return os;
    }

private:
    template<typename T>
    struct always_false : std::false_type {};
    uint8_t type; ///< The type of the event.
    std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData, int, std::string, sf::Vector2f, std::pair<size_t, sf::Vector2f>> payload; ///< The payload of the event.
};

#endif // EVENT_HPP
