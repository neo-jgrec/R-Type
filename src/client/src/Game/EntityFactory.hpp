#pragma once
#include "Components.hpp"
#include "../../../core/ecs/Registry/Registry.hpp"
#include <SFML/Graphics.hpp>

class EntityFactory {
public:
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position);
    static core::ecs::Entity createPlayerProjectile(core::ecs::Registry& registry, const sf::Vector2f& startPosition);
    static core::ecs::Entity createEnemy(core::ecs::Registry& registry, const sf::Vector2f& position);
};
