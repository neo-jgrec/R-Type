#include "Systems.hpp"

#include "Server.hpp"
#include "Components.hpp"
#include "EntityFactory.hpp"

void Systems::worldSystem(Server &server)
{
    core::GameEngine &gameEngine = server.getGameEngine();

    gameEngine.registry.add_system<World>(
        [&](const core::ecs::Entity &, World &world) {
            static float nextScrollUpdate = 0;

            world.scroll += static_cast<float>(world.speed) * gameEngine.delta_t;

            if (world.scroll < nextScrollUpdate)
                return;

            nextScrollUpdate = world.scroll + 75;
            if (rand() % 100 > 25)
                return;

            EntityFactory::createEnemy(server, static_cast<uint32_t>(world.scroll + 1920));
        });
}

void Systems::enemySystem(Server &server)
{
    core::GameEngine &gameEngine = server.getGameEngine();

    gameEngine.registry.add_system<core::ge::TransformComponent, Enemy>(
        [&](const core::ecs::Entity &entity, const core::ge::TransformComponent &transformComponent, const Enemy &enemy) {
            if (transformComponent.position.x > 0.f)
                return;

            std::cout << "Enemy " << static_cast<int>(enemy.id) << " died" << std::endl;
            gameEngine.registry.kill_entity(entity);
        });
}
