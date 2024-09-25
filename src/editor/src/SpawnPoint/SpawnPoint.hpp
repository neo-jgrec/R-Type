/*
** EPITECH PROJECT, 2024
** editor
** File description:
** SpawnPoint
*/

#ifndef SPAWNPOINT_HPP_
#define SPAWNPOINT_HPP_

#include <SFML/Graphics.hpp>

namespace Editor {
    class SpawnPoint {
        public:
            SpawnPoint(float x, float y);
            sf::Vector2f getPosition() const;
            void setPosition(float x, float y);
            void draw(sf::RenderWindow &window) const;

        private:
            sf::Vector2f _position;
    };
}

#endif /* !SPAWNPOINT_HPP_ */
