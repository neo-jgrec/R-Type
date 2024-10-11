/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Event
*/

#include "Event.hpp"

#include <utility>

Event::Event(uint8_t type, std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData, int, std::string> payload)
    : type(type), payload(std::move(payload)) {}

uint8_t Event::getType() const {
    return type;
}
const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData, int, std::string>& Event::getPayload() const {
    return payload;
}