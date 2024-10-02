/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Map
*/

#include "Map.hpp"
#include <fstream>
#include <iostream>

Map::Map(const std::string& filePath) {
    loadFromFile(filePath);
}

void Map::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    nlohmann::json jsonData;
    file >> jsonData;

    // Load map properties
    width = jsonData["width"];
    height = jsonData["height"];
    cellSize = jsonData["cellSize"];

    // Load tilesets
    for (const auto& tilesetData : jsonData["tileSets"]) {
        TileSet tileSet;
        tileSet.filePath = tilesetData["filePath"];
        tileSet.tileCount = tilesetData["tileCount"];
        tileSet.tileWidth = tilesetData["tileWidth"];
        tileSet.tileHeight = tilesetData["tileHeight"];
        tileSets.push_back(tileSet);
    }

    // Load tiles
    for (const auto& tileData : jsonData["tiles"]) {
        Tile tile;
        tile.tileIndex = tileData["tileIndex"];
        tile.x = tileData["x"];
        tile.y = tileData["y"];

        int packedCoords = packCoordinates(tile.x, tile.y);
        tiles[packedCoords] = tile;
    }
}

std::optional<Tile> Map::getTileAt(int x, int y) const {
    int packedCoords = packCoordinates(x, y);
    auto it = tiles.find(packedCoords);
    if (it != tiles.end()) {
        return it->second;
    }
    return std::nullopt;
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

int Map::getCellSize() const {
    return cellSize;
}

int Map::packCoordinates(int x, int y) const {
    return (x << 16) | y;
}

