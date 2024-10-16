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
#include <memory>

using json = nlohmann::json;

namespace Editor {
    class Map {
    public:
        Map();
        void loadMapConfig(const std::string &mapPath);
        void saveMap(const std::string &fileName);
        bool placeTile(int x, int y, int tileIndex);
        void removeTile(int x, int y);
        void createNewMap(int width, int height, int cellSize);

        [[nodiscard]]
        int getTile(int x, int y) const;
        [[nodiscard]]
        const Tile& getTileObject(int x, int y) const;
        [[nodiscard]]
        int getWidth() const;
        [[nodiscard]]
        int getHeight() const;
        [[nodiscard]]
        std::string getName() const;

        void setWidth(int width);
        void setHeight(int height);
        void setCellSize(int cellSize);
        void setName(const std::string& name);

        void loadTileSet(const std::string& filePath, int tileWidth, int tileHeight);
        void clearTileSets();
        void clearMap();

        [[nodiscard]]
        const Tile& getTileById(int tileId) const;
        [[nodiscard]]
        const std::vector<std::unique_ptr<TileSet>>& getTileSets() const;

        void draw(sf::RenderWindow& window, std::vector<sf::Vector2i>& selectedTiles) const;
        void drawPreviewTile(int x, int y, int tileIndex, sf::RenderWindow& window) const;

        [[nodiscard]]
        bool isPositionValid(int x, int y) const;

        Grid& getGrid();

        void setBackground(const std::string& backgroundPath);
        void setBackgroundHeight(int height);

        [[nodiscard]]
        std::string getBackgroundPath() const;
        [[nodiscard]]
        int getBackgroundHeight() const;

        [[nodiscard]]
        std::string getEditorVersion() const;

        void setMapEditorVersion(const std::string& version);
        [[nodiscard]]
        std::string getMapEditorVersion() const;

    private:
        std::string _name;
        std::string _filePath;
        int _width;
        int _height;
        int _cellSize;
        std::string _editorVersion;
        std::string _mapEditorVersion;
        std::vector<std::vector<std::unique_ptr<Tile>>> _tileMap;
        std::vector<std::unique_ptr<TileSet>> _tileSets;
        Grid _grid;
        std::string _backgroundPath;
        sf::Texture _backgroundTexture;
        sf::Sprite _backgroundSprite;
        int _backgroundHeight;

        void resizeMap();

        void drawTileStyle(sf::RenderWindow& window, const Tile& tile, sf::Vector2f position) const;
    };
}

#endif /* MAP_HPP_ */