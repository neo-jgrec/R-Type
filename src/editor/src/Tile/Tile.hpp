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

namespace Editor {
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
        private:
            sf::Sprite _sprite;
            int _id;
            bool _isDestructible = false;
    };
}

#endif /* !TILE_HPP_ */