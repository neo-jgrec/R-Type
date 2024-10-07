#include <iostream>
#include "Game.hpp"
#include "../../../game/Components.hpp"
#include "EntityFactory.hpp"

void Game::inputSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, InputStateComponent, ShootCounterComponent>
    ([&](core::ecs::Entity, core::ge::TransformComponent &transform, const VelocityComponent &vel, InputStateComponent &input, ShootCounterComponent &shootCounter) {
            if (input.up) {
                transform.position.y -= vel.dy;
                input.up = false;
            }
            if (input.down) {
                transform.position.y += vel.dy;
                input.down = false;
            }
            if (input.left) {
                transform.position.x -= vel.dx;
                input.left = false;
            }
            if (input.right) {
                transform.position.x += vel.dx;
                input.right = false;
            }
            if (input.fire) {
                if (shootCounter.shotCount >= 6) {
                    EntityFactory::createPlayerMissile(registry, transform);

                    shootCounter.shotCount = 0;
                } else {
                    EntityFactory::createPlayerProjectile(registry, transform);
    
                    shootCounter.shotCount++;
                }
                input.fire = false;
            }
    });
}

void Game::projectileMovementSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, Projectile>(
        [&](core::ecs::Entity, core::ge::TransformComponent &transform, VelocityComponent &velocity, Projectile&) {

            transform.position.x += velocity.dx;

            auto projectilesToCheck = registry.get_entities<Projectile>();

            for (const auto &projectile : projectilesToCheck) {
                float projectileWidth = transform.size.x * transform.scale.x;
                float projectileHeight = transform.size.y * transform.scale.y;
                if (projectileWidth > 800.0f || projectileWidth < 0.0f || projectileHeight > 600.0f || projectileHeight < 0.0f) {
                    registry.kill_entity(projectile);
                }
            }
        });
}

void Game::enemyMovementSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, Enemy>(
        [&](core::ecs::Entity, core::ge::TransformComponent &transform, VelocityComponent &velocity, Enemy&) {

            transform.position.x -= velocity.dx;

            auto enemiesToCheck = registry.get_entities<Enemy>();

            for (const auto &enemy : enemiesToCheck) {
                float enemyWidth = transform.size.x * transform.scale.x;
                float enemyHeight = transform.size.y * transform.scale.y;
                if (enemyWidth > 800.0f || enemyWidth < 0.0f || enemyHeight > 600.0f || enemyHeight < 0.0f) {
                    registry.kill_entity(enemy);
                }
            }
        });
}
