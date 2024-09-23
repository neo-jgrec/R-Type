#pragma once
#include "Components.hpp"
#include "../../../core/ecs/Registry/Registry.hpp"
#include <SFML/Graphics.hpp>

class EntityFactory {
public:
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry);
    // TODO: enemies, projectiles, etc.
};
