#include "Systems.hpp"

#include "Server.hpp"
#include "Components.hpp"
#include "EntityFactory.hpp"
#include "../../../game/RequestType.hpp"

void Systems::worldSystem(Server &server)
{
    core::GameEngine &gameEngine = server.getGameEngine();

    gameEngine.registry.add_system<World>(
        [&](const core::ecs::Entity &, World &world) {
            static float nextScrollUpdate = 0;

            world.scroll += static_cast<float>(world.speed) * gameEngine.delta_t;

            if (world.scroll < nextScrollUpdate)
                return;

            if (rand() % 100 > 25)
                return;

            EntityFactory::createEnemy(server, static_cast<uint32_t>(world.scroll));
        });
}

void Systems::enemySystem(Server &server)
{
    core::GameEngine &gameEngine = server.getGameEngine();

    gameEngine.registry.add_system<Enemy>(
        [&](const core::ecs::Entity &entity, const Enemy &enemy) {
            const auto &transformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(entity);
            if (transformComponent->position.x < 0.f) {
                std::cout << "Enemy " << static_cast<int>(enemy.id) << " died" << std::endl;
                gameEngine.registry.kill_entity(entity);
                return;
            }
            
            std::cout << "Enemy " << static_cast<int>(enemy.id) << " moved to " << transformComponent->position.x << std::endl;
            transformComponent->position.x -= 10 * gameEngine.delta_t;
        });
}

void Systems::projectileSystem(Server &server)
{
    core::GameEngine &gameEngine = server.getGameEngine();

    gameEngine.registry.add_system<Projectile>(
        [&](const core::ecs::Entity &entity, const Projectile &) {
            const auto &transformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(entity);
            transformComponent->position.x += 10 * gameEngine.delta_t;
        });
}
