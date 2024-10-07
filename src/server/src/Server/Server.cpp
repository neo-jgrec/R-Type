#include "Server.hpp"
#include "Components.hpp"
#include "Systems.hpp"
#include "EntityFactory.hpp"

Server::Server()
{
    _gameEngine.registry.register_component<Network>();
    _gameEngine.registry.register_component<World>();
    _gameEngine.registry.register_component<Player>();
    _gameEngine.registry.register_component<Enemy>();
    _gameEngine.registry.register_component<Projectile>();

    _world = EntityFactory::createWorld(_gameEngine.registry, "JY_map.json");

    Systems::worldSystem(_gameEngine.registry);
}

void Server::update()
{
    std::cout << "Updating server" << std::endl;
    _gameEngine.registry.run_system<Network, World>();
    _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::CollisionComponent>();
}

void Server::run()
{
    _players.push_back(EntityFactory::createPlayer(_gameEngine.registry));
    _enemies.push_back(EntityFactory::createEnemy(_gameEngine.registry));

    while (true) {
        update();
        sleep(1);
    }
}
