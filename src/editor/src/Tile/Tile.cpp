/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Tile
*/

#include "Tile.hpp"
#include <SFML/Graphics/Texture.hpp>

using namespace Editor;

Tile::Tile(
    const sf::Texture& texture,
    const sf::IntRect& textureRect,
    int id
) : _sprite(texture, textureRect), _id(id) {}

Tile::Tile() : _id(-1) {}

Tile::Tile(const Tile& other) : _sprite(other._sprite), _id(other._id), _isDestructible(other._isDestructible) {}

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

ImTextureID Tile::getTextureId() const {
    return reinterpret_cast<ImTextureID>(_sprite.getTexture()->getNativeHandle());
}

bool Tile::isDestructible() const {
    return _isDestructible;
}

void Tile::setDestructible(bool isDestructible) {
    _isDestructible = isDestructible;
}
