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

    _networkingService.addEvent(ConnectionAccept, [&](const GDTPHeader &, const std::vector<uint8_t> &, const asio::ip::udp::endpoint &endpoint) {
        std::cout << "New connection from " << endpoint << std::endl;
        for (uint8_t i = 0; i < 4; i++) {
            if (_players[i].has_value())
                continue;
            _players[i].emplace(EntityFactory::createPlayer(_gameEngine.registry, _networkingService, endpoint, i));
            return;
        }
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
    // _players.push_back(EntityFactory::createPlayer(_gameEngine.registry));
    _enemies.push_back(EntityFactory::createEnemy(_gameEngine.registry, _networkingService));

    _networkingService.run();
    while (_isRunning) {
        update();
        sleep(1);
    }
    std::cout << "Server stopped" << std::endl;
    _networkingService.stop();
}
