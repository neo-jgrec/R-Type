/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Map
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <nlohmann/json.hpp>

/**
 * @struct Tile
 * @brief Represents a tile in the map, with its index and position (x, y).
 */
struct Tile {
    int tileIndex;
    int x;
    int y;
};

/**
 * @struct TileSet
 * @brief Represents a tileset, including the image file path and tile dimensions.
 */
struct TileSet {
    std::string filePath;
    int tileCount;
    int tileWidth;
    int tileHeight;
};

/**
 * @class Map
 * @brief Represents a game map that loads and manages tiles and tilesets from a JSON file.
 */
class Map {
public:
    /**
     * @brief Constructs a Map object and loads data from the specified JSON file.
     * @param filePath The path to the JSON file containing map data.
     */
    Map(const std::string& filePath);

    /**
     * @brief Gets a tile at the specified (x, y) coordinates.
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @return An optional containing the Tile at the specified coordinates, or nullopt if no tile exists there.
     */
    std::optional<Tile> getTileAt(int x, int y) const;

    /**
     * @brief Gets the width of the map.
     * @return The width of the map.
     */
    int getWidth() const;

    /**
     * @brief Gets the height of the map.
     * @return The height of the map.
     */
    int getHeight() const;

    /**
     * @brief Gets the cell size of the map.
     * @return The cell size (width and height of a tile).
     */
    int getCellSize() const;

private:
    void loadFromFile(const std::string& filePath);

    int width;
    int height;
    int cellSize;

    std::vector<TileSet> tileSets;
    std::unordered_map<int, Tile> tiles; // Keys are (x, y) coordinates packed into an int

    int packCoordinates(int x, int y) const;
};


