/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Event
*/

#include "Event.hpp"

/**
 * @brief Constructor for an event with a PlayerMovement payload.
 */
Event::Event(uint8_t type, const PlayerMovement& movement)
    : type(type), payload(movement) {}

/**
 * @brief Constructor for an event with a ChatMessage payload.
 */
Event::Event(uint8_t type, const ChatMessage& message)
    : type(type), payload(message) {}

/**
 * @brief Constructor for an event with a PlayerShoot payload.
 */
Event::Event(uint8_t type, const PlayerShoot& shoot)
    : type(type), payload(shoot) {}

/**
 * @brief Constructor for an event with a PowerUpCollected payload.
 */
Event::Event(uint8_t type, const PowerUpCollected& powerUp)
    : type(type), payload(powerUp) {}

/**
 * @brief Constructor for an event with an EntityUpdate payload.
 */
Event::Event(uint8_t type, const EntityUpdate& update)
    : type(type), payload(update) {}

/**
 * @brief Constructor for an event without a payload.
 */
Event::Event(uint8_t type)
    : type(type), payload(NoData{}) {}

/**
 * @brief Returns the type of the event.
 */
uint8_t Event::getType() const {
    return type;
}

/**
 * @brief Returns the payload of the event.
 */
const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData>& Event::getPayload() const {
    return payload;
}


/**
 * @brief Constructor for an event with a PlayerHealthUpdate payload.
 */
Event::Event(uint8_t type, const PlayerHealthUpdate& healthUpdate)
    : type(type), payload(healthUpdate) {}

/**
 * @brief Constructor for an event with an EntitySpawn payload.
 */
Event::Event(uint8_t type, const EntitySpawn& spawn)
    : type(type), payload(spawn) {}

/**
 * @brief Constructor for an event with an EntityDestroy payload.
 */
Event::Event(uint8_t type, const EntityDestroy& destroy)
    : type(type), payload(destroy) {}