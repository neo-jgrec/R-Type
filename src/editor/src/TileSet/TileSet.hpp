/*
** EPITECH PROJECT, 2024
** editor
** File description:
** TileSet
*/

#ifndef TILESET_HPP_
#define TILESET_HPP_

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../Tile/Tile.hpp"
#include <imgui.h>

namespace Editor {
    class TileSet {
        public:
            TileSet(
                const std::string& texturePath,
                int tileWidth,
                int tileHeight,
                std::string name,
                int nextId
            );
            void loadFromFile(const std::string& texturePath);
            void sliceIntoTiles(int tileWidth, int tileHeight);

            [[nodiscard]] const Tile& getTile(int index) const;
            [[nodiscard]] size_t getTileCount() const;
            [[nodiscard]] const std::string& getName() const;
            [[nodiscard]] size_t getTilesLength() const;
            [[nodiscard]] const std::string& getFilePath() const;
            [[nodiscard]] sf::Vector2i getTileSize() const;
            [[nodiscard]] sf::Texture& getTexture();
            [[nodiscard]] ImTextureID getTextureID() const;
            [[nodiscard]] sf::Vector2f getTextureSize() const;
            [[nodiscard]] int getNextId() const;
            static void resetNextId();
        private:
            sf::Texture _texture;
            std::vector<std::unique_ptr<Tile>> _tiles;
            std::string _name;
            int _tileWidth;
            int _tileHeight;
            int _nextId;
    };
}

#endif /* !TILESET_HPP_ */