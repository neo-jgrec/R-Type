/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Event
*/

#include "Event.hpp"
#include <utility>

Event::Event(RequestType type, const GDTPHeader& header, std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShootEvent, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData, int, std::string, sf::Vector2f, std::pair<size_t, sf::Vector2f>> payload)
    : type(type), header(header), payload(std::move(payload)) {}

RequestType Event::getType() const
{
    return type;
}

const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShootEvent, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData, int, std::string, sf::Vector2f, std::pair<size_t, sf::Vector2f>>& Event::getPayload() const
{
    return payload;
}
