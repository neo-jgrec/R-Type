#pragma once
#include "Components.hpp"
#include "Globals.hpp"
#include <SFML/Graphics.hpp>
#include "../../../core/ecs/Registry/Registry.hpp"

namespace Systems {
    void movementSystem(core::ecs::Registry& registry);
    void renderSystem(core::ecs::Registry& registry, sf::RenderWindow& window);
    void positionSystem(core::ecs::Registry& registry);
    void animationSystem(core::ecs::Registry& registry);
    void projectileMovementSystem(core::ecs::Registry& registry);
}
