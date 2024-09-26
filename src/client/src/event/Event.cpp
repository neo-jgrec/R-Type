/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Event
*/

#include "Event.hpp"

/**
 * @brief Constructor for an event with a PlayerMovement payload.
 * @param type The type of the event.
 * @param movement The PlayerMovement payload.
 */
Event::Event(EventType type, const PlayerMovement& movement)
    : type(type), payload(movement) {}

/**
 * @brief Constructor for an event with a ChatMessage payload.
 * @param type The type of the event.
 * @param message The ChatMessage payload.
 */
Event::Event(EventType type, const ChatMessage& message)
    : type(type), payload(message) {}

/**
 * @brief Constructor for an event with a PlayerShoot payload.
 * @param type The type of the event.
 * @param shoot The PlayerShoot payload.
 */
Event::Event(EventType type, const PlayerShoot& shoot)
    : type(type), payload(shoot) {}

/**
 * @brief Constructor for an event without a payload.
 * @param type The type of the event.
 */
Event::Event(EventType type)
    : type(type), payload(NoData{}) {}

/**
 * @brief Returns the type of the event.
 * @return The EventType of the event.
 */
Event::EventType Event::getType() const {
    return type;
}

/**
 * @brief Returns the payload of the event.
 * @return A variant containing the event payload.
 */
const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, NoData>& Event::getPayload() const {
    return payload;
}
