/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Map
*/

#include "Map.hpp"
#include "../Exceptions.hpp"
#include <fstream>
#include <iomanip>

using namespace Editor;

Map::Map() : _width(0), _height(0), _cellSize(24), _grid(1, 1, 24) {}

void Map::loadMapConfig(const std::string &mapPath) {
    std::ifstream file(mapPath);
    if (!file.is_open())
        throw Editor::Exception("Failed to open map file: " + mapPath);

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& e) {
        throw Editor::Exception("Failed to parse JSON file: " + std::string(e.what()));
    }

    _width = data["width"];
    _height = data["height"];
    _cellSize = data["cellSize"];
    _tileMap.resize(_height, std::vector<int>(_width, -1));
    _grid.setGridSize(_width, _height);
    _grid.setCellSize(_cellSize);

    if (data.contains("tiles")) {
        for (const auto& tile : data["tiles"]) {
            int x = tile["x"];
            int y = tile["y"];
            int tileIndex = tile["tileIndex"];
            _tileMap[y][x] = tileIndex;
        }
    }
}

void Map::saveMap(const std::string &fileName) {
    json mapData;
    mapData["width"] = _width;
    mapData["height"] = _height;

    json tilesArray = json::array();
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            int tileIndex = _tileMap[y][x];
            if (tileIndex != -1) {
                json tile;
                tile["x"] = x;
                tile["y"] = y;
                tile["tileIndex"] = tileIndex;
                tilesArray.push_back(tile);
            }
        }
    }
    mapData["tiles"] = tilesArray;

    json tileSetsArray = json::array();
    for (const auto& tileSet : _tileSets) {
        json tileSetData;
        tileSetData["filePath"] = tileSet->getFilePath();
        tileSetData["tileWidth"] = tileSet->getTileSize().x;
        tileSetData["tileHeight"] = tileSet->getTileSize().y;
        tileSetsArray.push_back(tileSetData);
    }
    mapData["tileSets"] = tileSetsArray;

    std::ofstream file(fileName);
    if (file.is_open()) {
        file << std::setw(4) << mapData << std::endl;
        file.close();
    } else {
        throw Editor::Exception("Failed to open file for writing: " + fileName);
    }
}

void Map::placeTile(int x, int y, int tileIndex) {
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        _tileMap[y][x] = tileIndex;
    }
}

void Map::removeTile(int x, int y) {
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        _tileMap[y][x] = -1;
    }
}

int Map::getTile(int x, int y) const {
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        return _tileMap[y][x];
    }
    return -1;
}

int Map::getWidth() const {
    return _width;
}

int Map::getHeight() const {
    return _height;
}

void Map::loadTileSet(const std::string& filePath, int tileWidth, int tileHeight) {
    _tileSets.push_back(std::make_unique<TileSet>(filePath, tileWidth, tileHeight, filePath));
}

const Tile& Map::getTileById(int tileId) const {
    for (const auto& tileSet : _tileSets) {
        for (int i = 0; i < static_cast<int>(tileSet->getTileCount()); ++i) {
            const Tile& tile = tileSet->getTile(i);
            if (tile.getId() == tileId) {
                return tile;
            }
        }
    }
    throw Editor::Exception("Tile not found");
}

const std::vector<std::unique_ptr<TileSet>>& Map::getTileSets() const {
    return _tileSets;
}

void Map::draw(sf::RenderWindow& window) const {
    _grid.draw(window);
    if (_tileMap.empty())
        return;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            int tileId = _tileMap[y][x];
            if (tileId == -1)
                continue;
            const auto& tile = getTileById(tileId);
            sf::Vector2f position(static_cast<float>(x * _cellSize), static_cast<float>(y * _cellSize));
            tile.draw(window, position.x, position.y);
        }
    }
}

Grid& Map::getGrid() {
    return _grid;
}
