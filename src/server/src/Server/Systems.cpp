#include "Systems.hpp"

#include "Server.hpp"
#include "Components.hpp"
#include "EntityFactory.hpp"

void Systems::worldSystem(Server &server)
{
    core::GameEngine &gameEngine = server.getGameEngine();

    gameEngine.registry.add_system<core::ge::TransformComponent, World>(
        [&](const core::ecs::Entity &, const core::ge::TransformComponent &transformComponent,World &world) {
            const time_t currentTime = std::time(nullptr);
            if (currentTime - world.lastTimeEnemySpawned < world.enemySpawnRate)
                return;

            world.lastTimeEnemySpawned = currentTime;
            EntityFactory::createEnemy(server, static_cast<uint32_t>(transformComponent.position.x) + world.size.first + 100);
        });
}
