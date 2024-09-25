/*
** EPITECH PROJECT, 2024
** editor
** File description:
** TileSet
*/


#include "TileSet.hpp"
#include "../Exceptions.hpp"

using namespace Editor;

int TileSet::_nextId = 1;

TileSet::TileSet(const std::string& texturePath, int tileWidth, int tileHeight, std::string name) : _name(std::move(name)) {
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
