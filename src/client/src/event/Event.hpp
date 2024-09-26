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
};

/**
 * @struct ChatMessage
 * @brief Represents the data for a chat message event.
 */
struct ChatMessage {
    uint32_t playerId;
    std::string message;
};

/**
 * @struct PlayerShoot
 * @brief Represents the data for a player shooting event.
 */
struct PlayerShoot {
    uint32_t playerId;
    uint8_t direction;
    uint8_t weaponType;
};

/**
 * @struct NoData
 * @brief Represents an event that does not have a payload.
 */
struct NoData {};

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
     * @enum EventType
     * @brief Enum representing the different types of events that can occur.
     */
    typedef GDTPMessageType EventType;

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
    const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, NoData>& getPayload() const;

private:
    EventType type; ///< The type of the event.
    std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, NoData> payload; ///< The payload of the event.
};

#endif // EVENT_HPP
