#include "Game.hpp"
#include "../../../game/Components.hpp"
#include "EntityFactory.hpp"

void Game::inputSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, InputStateComponent, Player>
    ([&](core::ge::TransformComponent &transform, const VelocityComponent &vel, InputStateComponent &input, Player&) {
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
                sf::Vector2f projectilePosition = transform.position;

                float playerWidth = transform.size.x * transform.scale.x;
                float playerHeight = transform.size.y * transform.scale.y;

                projectilePosition.x += playerWidth ;

                float projectileHeight = 5.0f;
                projectilePosition.y += (playerHeight / 2.0f) - (projectileHeight / 2.0f);
                EntityFactory::createPlayerProjectile(registry, projectilePosition);
                input.fire = false;
            }
    });
}

void Game::projectileMovementSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, Projectile>(
        [&](core::ge::TransformComponent &transform, VelocityComponent &velocity, Projectile&) {

            transform.position.x += velocity.dx;

            auto projectilesToCheck = registry.get_entities<Projectile>();

            for (const auto &projectile : projectilesToCheck) {
                if (transform.position.x > 800.0f || transform.position.x < 0.0f) {
                    registry.kill_entity(projectile);
                }
            }
        });
}

void Game::enemyMovementSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, Enemy>(
        [&](core::ge::TransformComponent &transform, VelocityComponent &velocity, Enemy&) {

            transform.position.x -= velocity.dx;

            auto enemiesToCheck = registry.get_entities<Enemy>();

            for (const auto &enemy : enemiesToCheck) {
                if (transform.position.x > 800.0f || transform.position.x < 0.0f) {
                    registry.kill_entity(enemy);
                }
            }
        });
}
