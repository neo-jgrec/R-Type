#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include "../../../core/ecs/Registry/Registry.hpp"
#include <SFML/Graphics.hpp>

class EntityFactory {
public:
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position);
    static core::ecs::Entity createPlayerProjectile(core::ecs::Registry& registry, const sf::Vector2f& startPosition);
    static core::ecs::Entity createEnemy(core::ecs::Registry& registry, const sf::Vector2f& position);
    static core::ecs::Entity createButton(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void()>& onClick, int scene);
};

#endif // ENTITY_FACTORY_HPP
