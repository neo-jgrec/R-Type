/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Entiry
*/

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <SFML/Graphics.hpp>
#include <string>

namespace Editor {
    class Entity {
        public:
            Entity(float x, float y, std::string type);
            virtual ~Entity() = default;

            virtual void draw(sf::RenderWindow& window) const = 0;
            virtual void setPosition(float x, float y);
            virtual void setPosition(const sf::Vector2f& position);

            [[nodiscard]]
            virtual sf::Vector2f getPosition() const;
            virtual void setOpacity(float opacity);

            [[nodiscard]]
            virtual std::string getType() const;

        protected:
            sf::Vector2f _position;
            float _opacity;
            std::string _type;
    };
}

#endif /* !ENTITY_HPP_ */