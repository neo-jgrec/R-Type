/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Enemy
*/

#include "Enemy.hpp"

#include <utility>

using namespace Editor;


Enemy::Enemy(float x, float y, std::string type) : Entity(x, y, std::move(type)) {}

sf::Vector2f Enemy::getSize() {
    return {20, 20};
}

void Enemy::draw(sf::RenderWindow &window) const {
    sf::RectangleShape shape(getSize());
    shape.setPosition(_position);
    shape.setFillColor(sf::Color(255, 0, 0, static_cast<int>(_opacity * 255)));
    window.draw(shape);
}
