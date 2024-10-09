#include "Server.hpp"
#include "Components.hpp"
#include "Systems.hpp"
#include "EntityFactory.hpp"

#include "../../../game/MessageType.hpp"

Server::Server()
{
    _gameEngine.registry.register_component<Network>();
    _gameEngine.registry.register_component<ConnectionHub>();
    _gameEngine.registry.register_component<World>();
    _gameEngine.registry.register_component<Player>();
    _gameEngine.registry.register_component<Enemy>();
    _gameEngine.registry.register_component<Projectile>();

    _connectionHub = EntityFactory::createConnectionHub(_gameEngine.registry, _networkingService, _players);
    _world = EntityFactory::createWorld(_gameEngine.registry, _networkingService, "JY_map.json");

    Systems::worldSystem(_gameEngine.registry);

    _networkingService.addEvent(EntitySpawn, [&](const GDTPHeader &, const std::vector<uint8_t> &, const asio::ip::udp::endpoint &) {
        std::cout << "EntitySpawn" << std::endl;
    });
}

void Server::update()
{
    std::cout << "↻ Updating server" << std::endl;
    _gameEngine.registry.run_system<Network, World>();
    _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::CollisionComponent>();
}

void Server::run()
{
    std::cout << "Hello, World!" << std::endl;

    // _players.push_back(EntityFactory::createPlayer(_gameEngine.registry));
    _enemies.push_back(EntityFactory::createEnemy(_gameEngine.registry, _networkingService));

    _networkingService.run();
    while (true) {
        update();
        #ifdef _WIN32
            _sleep(1000);
        #else
            sleep(1);
        #endif
    }
    _networkingService.stop();
}
