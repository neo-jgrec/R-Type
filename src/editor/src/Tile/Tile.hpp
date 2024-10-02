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
            Tile(const sf::Texture& texture, const sf::IntRect& textureRect, int id);
            void draw(sf::RenderWindow& window, float x, float y) const;

            [[nodiscard]] const sf::Sprite& getSprite() const;

            [[nodiscard]] int getId() const;
            [[nodiscard]] ImTextureID getTextureId() const;
        private:
            sf::Sprite _sprite;
            int _id;
    };
}

#endif /* !TILE_HPP_ */