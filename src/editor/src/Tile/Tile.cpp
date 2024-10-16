/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Tile
*/

#include "Tile.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <algorithm>

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

void Tile::setId(int id) {
    _id = id;
}

void Tile::update(
    const sf::Texture& texture,
    const sf::IntRect& textureRect,
    int id
) {
    _sprite.setTexture(texture);
    _sprite.setTextureRect(textureRect);
    _id = id;
}

void Tile::addTag(const std::string& tag) {
    if (std::find(_tags.begin(), _tags.end(), tag) == _tags.end()) {
        _tags.push_back(tag);
    }
}

void Tile::removeTag(const std::string& tag) {
    _tags.erase(std::remove(_tags.begin(), _tags.end(), tag), _tags.end());
}

const std::vector<std::string>& Tile::getTags() const {
    return _tags;
}

bool Tile::hasTag(const std::string& tag) const {
    return std::find(_tags.begin(), _tags.end(), tag) != _tags.end();
}

TileStyle Tile::getStyle() const {
    return _style;
}

void Tile::setStyle(TileStyle style) {
    _style = style;
}
