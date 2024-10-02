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
#include <iostream>

using namespace Editor;

Map::Map() :
    _name("Untitled"),
    _width(1),
    _height(1),
    _cellSize(24),
    _grid(1, 1, 24)
{
    _tileMap.resize(_height, std::vector<int>(_width, -1));
    _grid.setGridSize(_width * _cellSize, _height * _cellSize);
    _grid.setCellSize(_cellSize);
}

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

    if (!data.contains("cellSize") || !data.contains("width") || !data.contains("height"))
        throw Editor::Exception("Missing required fields in map configuration");

    if (!data["cellSize"].is_number_integer() || !data["width"].is_number_integer() || !data["height"].is_number_integer())
        throw Editor::Exception("Invalid data types for cellSize, width, or height");

    if (!data.contains("name"))
        throw Editor::Exception("Missing required field 'name' in map configuration");

    _cellSize = data["cellSize"];
    _width = data["width"];
    _height = data["height"];
    _name = data["name"];

    if (_cellSize <= 0 || _width <= 0 || _height <= 0)
        throw Editor::Exception("Invalid values for cellSize, width, or height");

    _tileMap.resize(_height, std::vector<int>(_width, -1));
    _grid.setGridSize(_width * _cellSize, _height * _cellSize);
    _grid.setCellSize(_cellSize);

    if (data.contains("tiles")) {
        if (!data["tiles"].is_array())
            throw Editor::Exception("Invalid 'tiles' field: expected an array");
        for (const auto& tile : data["tiles"]) {
            if (!tile.contains("x") || !tile.contains("y") || !tile.contains("tileIndex"))
                throw Editor::Exception("Invalid tile data: missing x, y, or tileIndex");
            int x = tile["x"];
            int y = tile["y"];
            int tileIndex = tile["tileIndex"];
            if (x < 0 || x >= _width / _cellSize || y < 0 || y >= _height / _cellSize)
                throw Editor::Exception("Invalid tile position: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
            _tileMap[y][x] = tileIndex;
            std::cout << "Tile at (" << x << ", " << y << "): " << tileIndex << std::endl;
        }
    }

    if (data.contains("tileSets")) {
        if (!data["tileSets"].is_array())
            throw Editor::Exception("Invalid 'tileSets' field: expected an array");
        for (const auto& tileSetData : data["tileSets"]) {
            if (!tileSetData.contains("filePath") || !tileSetData.contains("tileWidth") || !tileSetData.contains("tileHeight"))
                throw Editor::Exception("Invalid tileSet data: missing filePath, tileWidth, or tileHeight");
            std::string filePath = tileSetData["filePath"];
            int tileWidth = tileSetData["tileWidth"];
            int tileHeight = tileSetData["tileHeight"];
            if (tileWidth <= 0 || tileHeight <= 0)
                throw Editor::Exception("Invalid tileSet dimensions: tileWidth and tileHeight must be positive");
            loadTileSet(filePath, tileWidth, tileHeight);
            std::cout << "Tile set loaded: " << filePath << std::endl;
        }
    }
}

void Map::createNewMap(int width, int height, int cellSize) {
    _width = width * cellSize;
    _height = height * cellSize;
    _cellSize = cellSize;
    _tileMap.clear();
    _tileSets.clear();
    _tileMap.resize(_height, std::vector<int>(_width, -1));
    _grid.setGridSize(_width * _cellSize, _height * _cellSize);
    _grid.setCellSize(cellSize);
}

void Map::clearMap() {
    _tileMap.clear();
    _tileSets.clear();
    _width = 0;
    _height = 0;
    _cellSize = 0;
}

void Map::saveMap(const std::string &fileName) {
    json mapData;
    mapData["width"] = _width;
    mapData["height"] = _height;
    mapData["cellSize"] = _cellSize;
    mapData["editorVersion"] = "0.1";
    mapData["name"] = _name;

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
        tileSetData["tileCount"] = tileSet->getTileCount();
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
    if (!isPositionValid(x, y))
        return;
    _tileMap[y][x] = tileIndex;
}

void Map::removeTile(int x, int y) {
    if (!isPositionValid(x, y))
        return;
    _tileMap[y][x] = -1;
}

int Map::getTile(int x, int y) const {
    if (isPositionValid(x, y))
        return _tileMap[y][x];
    return -1;
}

int Map::getWidth() const {
    return _width;
}

int Map::getHeight() const {
    return _height;
}

void Map::loadTileSet(const std::string& filePath, int tileWidth, int tileHeight) {
    int actualTileSetsTilesCount = 0;
    for (const auto& tileSet : _tileSets)
        actualTileSetsTilesCount += static_cast<int>(tileSet->getTileCount());
    int nextId = actualTileSetsTilesCount + 1;
    _tileSets.push_back(std::make_unique<TileSet>(filePath, tileWidth, tileHeight, filePath, nextId));
}

void Map::clearTileSets() {
    _tileSets.clear();
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

void Map::drawPreviewTile(int x, int y, int tileIndex, sf::RenderWindow& window) const {
    if (!isPositionValid(x, y))
        return;
    const auto& tile = getTileById(tileIndex);
    sf::Vector2f position(static_cast<float>(x * _cellSize), static_cast<float>(y * _cellSize));
    tile.draw(window, position.x, position.y);
}

bool Map::isPositionValid(int x, int y) const {
    return x >= 0 && x < _width * _cellSize && y >= 0 && y < _height * _cellSize;
}

void Map::setWidth(int width) {
    _width = width;
    _tileMap.resize(_height, std::vector<int>(_width, -1));
    _grid.setGridSize(_width, _height);
}

void Map::setHeight(int height) {
    _height = height;
    _tileMap.resize(_height, std::vector<int>(_width, -1));
    _grid.setGridSize(_width, _height);
}

void Map::setCellSize(int cellSize) {
    _cellSize = cellSize;
    _grid.setCellSize(_cellSize);
}

void Map::setName(const std::string& name) {
    _name = name;
}

std::string Map::getName() const {
    return _name;
}
