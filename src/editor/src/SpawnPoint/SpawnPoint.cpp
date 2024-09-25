/*
** EPITECH PROJECT, 2024
** editor
** File description:
** SpawnPoint
*/

#include "SpawnPoint.hpp"

using namespace Editor;

SpawnPoint::SpawnPoint(float x, float y) : _position(x, y) {}

sf::Vector2f SpawnPoint::getPosition() const {
    return _position;
}

void SpawnPoint::setPosition(float x, float y) {
    _position.x = x;
    _position.y = y;
}

void SpawnPoint::draw(sf::RenderWindow &window) const {
    sf::CircleShape shape(5);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(_position);
    window.draw(shape);
}
