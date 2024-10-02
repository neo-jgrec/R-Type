/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Enemy
*/

#ifndef ENEMY_HPP_
#define ENEMY_HPP_

#include <SFML/Graphics.hpp>
#include <string>
#include "../Entity/Entity.hpp"

namespace Editor {
    class Enemy : public Entity {
        public:
            Enemy(float x, float y, std::string type);

            void draw(sf::RenderWindow &window) const override;
            [[nodiscard]]
            static sf::Vector2f getSize();
    };
}

#endif /* !ENEMY_HPP_ */
