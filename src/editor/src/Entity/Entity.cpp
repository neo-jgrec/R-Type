/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Entity
*/

#include "Entity.hpp"

using namespace Editor;

Entity::Entity(float x, float y, std::string type)
    : _position(x, y), _opacity(1.0f), _type(std::move(type))
{
}

void Entity::setPosition(float x, float y)
{
    _position.x = x;
    _position.y = y;
}

void Entity::setPosition(const sf::Vector2f& position)
{
    _position = position;
}

sf::Vector2f Entity::getPosition() const
{
    return _position;
}

void Entity::setOpacity(float opacity)
{
    _opacity = opacity;
}

std::string Entity::getType() const
{
    return _type;
}
