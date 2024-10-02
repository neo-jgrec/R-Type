/*
** EPITECH PROJECT, 2024
** editor
** File description:
** SpawnPoint
*/

#ifndef SPAWNPOINT_HPP_
#define SPAWNPOINT_HPP_

#include <SFML/Graphics.hpp>
#include "../Entity/Entity.hpp"

namespace Editor {
    class SpawnPoint : public Entity {
        public:
            SpawnPoint(float x, float y);

            void draw(sf::RenderWindow &window) const override;
            [[nodiscard]]
            static sf::Vector2f getSize();

        private:
    };
}

#endif /* !SPAWNPOINT_HPP_ */
