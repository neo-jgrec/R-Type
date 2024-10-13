/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Event
*/

#include "Event.hpp"
#include <utility>

Event::Event(RequestType type, const GDTPHeader& header, std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShootEvent, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData, int, std::string, sf::Vector2u, std::pair<std::uint8_t, sf::Vector2u>, std::uint32_t, std::uint8_t> payload)
    : type(type), header(header), payload(std::move(payload)) {}

RequestType Event::getType() const
{
    return type;
}

const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShootEvent, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData, int, std::string, sf::Vector2u, std::pair<std::uint8_t, sf::Vector2u>, std::uint32_t, std::uint8_t>& Event::getPayload() const
{
    return payload;
}
