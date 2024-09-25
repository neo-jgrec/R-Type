/*
** EPITECH PROJECT, 2024
** editor
** File description:
** SpawnPoint
*/

#include "SpawnPoint.hpp"

using namespace Editor;

SpawnPoint::SpawnPoint(float x, float y) : Entity(x, y, "Spawn Point") {}

sf::Vector2f SpawnPoint::getSize() {
    return {10, 10};
}

void SpawnPoint::draw(sf::RenderWindow &window) const {
    sf::CircleShape shape(5);
    shape.setPosition(_position);
    shape.setFillColor(sf::Color(255, 0, 0, static_cast<int>(_opacity * 255)));
    window.draw(shape);
}
