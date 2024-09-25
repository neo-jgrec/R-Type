/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Tile
*/

#include "Tile.hpp"

using namespace Editor;

Tile::Tile(
    const sf::Texture& texture,
    const sf::IntRect& textureRect,
    int id
) : _sprite(texture, textureRect), _id(id) {}

void Tile::draw(sf::RenderWindow& window, float x, float y) const {
    sf::Sprite tempSprite = _sprite;
    tempSprite.setPosition(x, y);
    window.draw(tempSprite);
}

const sf::Sprite& Tile::getSprite() const {
    return _sprite;
}

int Tile::getId() const {
    return _id;
}
