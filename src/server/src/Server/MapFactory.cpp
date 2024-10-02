/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MapFactory
*/

#include "MapFactory.hpp"

MapData MapFactory::loadMap(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    nlohmann::json jsonMap;
    file >> jsonMap;

    MapData tileMapData;
    tileMapData.width = jsonMap["width"];
    tileMapData.height = jsonMap["height"];

    tileMapData.tiles = std::vector<std::vector<uint8_t>>(tileMapData.width, std::vector<uint8_t>(tileMapData.height, 0));

    for (const auto& tile : jsonMap["tiles"]) {
        int x = tile["x"];
        int y = tile["y"];
        int tileIndex = tile["tileIndex"];

        if (x < 0 || x >= tileMapData.width || y < 0 || y >= tileMapData.height) {
            throw std::out_of_range("Tile coordinates out of bounds");
        }

        tileMapData.tiles[x][y] = static_cast<uint8_t>(tileIndex);
    }

    return tileMapData;
}
