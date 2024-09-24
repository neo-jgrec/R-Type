#pragma once
#include "Components.hpp"
#include "../../../core/ecs/Registry/Registry.hpp"
#include <SFML/Graphics.hpp>

class EntityFactory {
public:
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry);
    static core::ecs::Entity createPlayerProjectile(core::ecs::Registry& registry, const sf::Vector2f& startPosition);
    // TODO: enemies, projectiles, etc.
};
