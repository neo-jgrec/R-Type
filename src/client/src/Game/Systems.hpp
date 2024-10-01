#pragma once
#include "Components.hpp"
#include "Globals.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "../../../core/ecs/Registry/Registry.hpp"
#include "../../../core/ecs/Entity/Entity.hpp"

namespace Systems {
    void inputSystem(core::ecs::Registry& registry);
    void renderSystem(core::ecs::Registry& registry, sf::RenderWindow& window);
    void positionSystem(core::ecs::Registry& registry);
    void animationSystem(core::ecs::Registry& registry);
    void projectileMovementSystem(core::ecs::Registry& registry);
    void collisionSystem(core::ecs::Registry& registry);
    void enemyMovementSystem(core::ecs::Registry& registry, sf::RenderWindow& window);
    void soundSystem(core::ecs::Registry& registry);
}
