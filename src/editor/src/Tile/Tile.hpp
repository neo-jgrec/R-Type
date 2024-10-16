/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Tile
*/

#ifndef TILE_HPP_
#define TILE_HPP_

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <vector>
#include <string>

namespace Editor {

    enum class TileStyle {
        DEFAULT,
        DESTRUCTIBLE,
        SPAWN,
    };

    class Tile {
        public:
            Tile();
            Tile(const sf::Texture& texture, const sf::IntRect& textureRect, int id);
            Tile(const Tile& other);

            void draw(sf::RenderWindow& window, float x, float y) const;

            [[nodiscard]] const sf::Sprite& getSprite() const;

            void setId(int id);
            [[nodiscard]] int getId() const;
            [[nodiscard]] ImTextureID getTextureId() const;
            [[nodiscard]] bool isDestructible() const;
            void setDestructible(bool isDestructible);
            void update(
                const sf::Texture& texture,
                const sf::IntRect& textureRect,
                int id
            );
            void addTag(const std::string& tag);
            void removeTag(const std::string& tag);
            [[nodiscard]] const std::vector<std::string>& getTags() const;
            [[nodiscard]] bool hasTag(const std::string& tag) const;

            [[nodiscard]] TileStyle getStyle() const;
            void setStyle(TileStyle style);
        private:
            sf::Sprite _sprite;
            int _id;
            bool _isDestructible = false;
            std::vector<std::string> _tags;
            TileStyle _style = TileStyle::DEFAULT;
    };
}

#endif /* !TILE_HPP_ */
