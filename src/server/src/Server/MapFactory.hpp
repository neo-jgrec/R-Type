/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MapFactory
*/

#pragma once

#ifndef MAPFACTORY_HPP
    #define MAPFACTORY_HPP

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

/**
 * @struct MapData
 * @brief A structure that holds the map information.
 *
 * This structure stores the map dimensions (width, height) and a 2D grid of tiles, where each tile
 * is represented as a `uint8_t`. The grid is stored as a vector of vectors.
 */
struct MapData {
    int width;  ///< The width of the map in tiles
    int height; ///< The height of the map in tiles
    std::vector<std::vector<uint8_t>> tiles; ///< 2D grid representing the map tiles
};

/**
 * @class MapFactory
 * @brief A factory class responsible for loading and generating map data from a JSON file.
 *
 * The MapFactory provides static methods to load a map from a JSON file and return a `MapData`
 * structure, which contains the map's size and tiles.
 *
 * Example usage:
 * @code
 * MapData map = MapFactory::loadMap("map.json");
 * @endcode
 */
class MapFactory {
public:
    /**
     * @brief Loads a map from a JSON file and returns a `MapData` structure.
     *
     * This static function reads a map file in JSON format and generates a `MapData` structure,
     * which includes the map's width, height, and a 2D grid of tiles. Each tile is represented by
     * a `uint8_t`, where the value corresponds to the tile's index or type.
     *
     * @param filePath The path to the JSON file containing the map data.
     * @return A `MapData` structure containing the map's dimensions and tile data.
     * @throws std::runtime_error If the file cannot be opened or if the JSON format is invalid.
     *
     * Example:
     * @code
     * MapData map = MapFactory::loadMap("map.json");
     * std::cout << "Map width: " << map.width << ", height: " << map.height << std::endl;
     * @endcode
     */
    static MapData loadMap(const std::string& filePath);
};

#endif //MAPFACTORY_HPP
