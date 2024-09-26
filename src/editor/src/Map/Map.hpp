/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Map
*/

#ifndef MAP_HPP_
#define MAP_HPP_

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include "../TileSet/TileSet.hpp"
#include "../Grid/Grid.hpp"

using json = nlohmann::json;

namespace Editor {
    class Map {
    public:
        Map();
        void loadMapConfig(const std::string &mapPath);
        void saveMap(const std::string &fileName);
        void placeTile(int x, int y, int tileIndex);
        void removeTile(int x, int y);

        [[nodiscard]]
        int getTile(int x, int y) const;
        [[nodiscard]]
        int getWidth() const;
        [[nodiscard]]
        int getHeight() const;

        void loadTileSet(const std::string& filePath, int tileWidth, int tileHeight);

        [[nodiscard]]
        const Tile& getTileById(int tileId) const;
        [[nodiscard]]
        const std::vector<std::unique_ptr<TileSet>>& getTileSets() const;

        void draw(sf::RenderWindow& window) const;

        Grid& getGrid();

    private:
        int _width;
        int _height;
        int _cellSize;
        std::vector<std::vector<int>> _tileMap;
        std::vector<std::unique_ptr<TileSet>> _tileSets;
        Grid _grid;
    };
}

#endif /* MAP_HPP_ */