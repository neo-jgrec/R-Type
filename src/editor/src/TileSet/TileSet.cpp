/*
** EPITECH PROJECT, 2024
** editor
** File description:
** TileSet
*/


#include "TileSet.hpp"
#include "../Exceptions.hpp"

using namespace Editor;

TileSet::TileSet(const std::string& texturePath, int tileWidth, int tileHeight, std::string name, int nextId) : _name(std::move(name)), _tileWidth(tileWidth), _tileHeight(tileHeight), _nextId(nextId) {
    loadFromFile(texturePath);
    sliceIntoTiles(tileWidth, tileHeight);
}

void TileSet::loadFromFile(const std::string& texturePath) {
    if (!_texture.loadFromFile(texturePath)) {
        throw Editor::Exception("Failed to load texture: " + texturePath);
    }
}

void TileSet::sliceIntoTiles(int tileWidth, int tileHeight) {
    sf::Vector2u textureSize = _texture.getSize();
    int columns = static_cast<int>(textureSize.x / tileWidth);
    int rows = static_cast<int>(textureSize.y / tileHeight);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            sf::IntRect tileRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);
            _tiles.push_back(std::make_unique<Tile>(_texture, tileRect, _nextId++));
        }
    }
}

const Tile& TileSet::getTile(int index) const {
    if (index < 0 || index >= static_cast<int>(_tiles.size())) {
        throw Editor::Exception("Tile index out of range");
    }
    return *_tiles[index];
}

size_t TileSet::getTileCount() const {
    return _tiles.size();
}

const std::string& TileSet::getName() const {
    return _name;
}

size_t TileSet::getTilesLength() const {
    return _tiles.size();
}

const std::string& TileSet::getFilePath() const {
    return _name;
}

sf::Vector2i TileSet::getTileSize() const {
    return {_tileWidth, _tileHeight};
}

sf::Texture& TileSet::getTexture() {
    return _texture;
}

ImTextureID TileSet::getTextureID() const {
    return reinterpret_cast<ImTextureID>(_texture.getNativeHandle());
}

sf::Vector2f TileSet::getTextureSize() const {
    return {static_cast<float>(_texture.getSize().x), static_cast<float>(_texture.getSize().y)};
}

int TileSet::getNextId() const {
    return _nextId;
}
