#include "Server.hpp"
#include "Components.hpp"
#include "EntityFactory.hpp"
#include "Systems.hpp"
#include "EventFactory.hpp"

bool Server::asPlayerConnected()
{
    return std::ranges::any_of(_playersConnection, [](const auto &player) { return player.has_value(); });
}

void Server::sendRequestToPlayers(const uint8_t requestType, const std::vector<uint8_t> &payload)
{
    for (const auto &playerEntity : _playersConnection) {
        if (!playerEntity.has_value())
            continue;
        _networkingService.sendRequest(
            *playerEntity.value(),
            requestType,
            payload);
    }
}

void Server::sendRequestToPlayers(const uint8_t requestType, const std::vector<uint8_t> &payload, const uint8_t selfPlayer)
{
    for (uint8_t i = 0; i < 4; i++) {
        if (!_playersConnection[i].has_value() || i == selfPlayer)
            continue;
        _networkingService.sendRequest(
            *_playersConnection[i].value(),
            requestType,
            payload);
    }
}


Server::Server()
{
    _gameEngine.registry.register_component<Network>();
    _gameEngine.registry.register_component<World>();
    _gameEngine.registry.register_component<Player>();
    _gameEngine.registry.register_component<Enemy>();
    _gameEngine.registry.register_component<Projectile>();

    _configManager.parse("assets/Data/config.json");

    Systems::worldSystem(*this);

    EventFactory::gameStarted(*this);
    EventFactory::playerConnected(*this);
    EventFactory::playerDisconnected(*this);
    EventFactory::playerMove(*this);
    EventFactory::playerShoot(*this);
}

void Server::start()
{
    static bool asGameStarted = false;
    if (asGameStarted)
        return;
    asGameStarted = true;

    std::cout << "Game starting" << std::endl;

    const core::ecs::Entity world = EntityFactory::createWorld(*this, "JY_map.json");
    if (const auto spawnPoints = _gameEngine.registry.get_component<World>(world)->spawnPoints; spawnPoints.empty()) {
        std::cerr << "Error: No spawn points available." << std::endl;
        return;
    }

    for (uint8_t i = 0; i < 4; i++) {
        if (!_playersConnection[i].has_value())
            continue;
        _players[i] = EntityFactory::createPlayer(*this, i);
    }

    _gameEngine.clock.restart();
    _gameState = GAME;
}

void Server::update()
{
    std::lock_guard lock(registry_mutex);

    _gameEngine.registry.run_system<core::ge::TransformComponent, World>();
    _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::VelocityComponent>();
    _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::CollisionComponent>();
}

void Server::run()
{
    sf::Time elapsed;

    while (true) {
        switch (_gameState) {
            case STARTING:
                std::cout << "Server started" << std::endl;
                _networkingService.run();
                _gameState = WAITING_CONNECTION;
                continue;

            case WAITING_CONNECTION:
            case LOBBY:
                break;

            case GAME:
                elapsed = _gameEngine.clock.restart();
                _gameEngine.delta_t = elapsed.asSeconds();

                update();
                break;

            case STOPPING:
                std::cout << "Server stopped" << std::endl;
                _networkingService.stop();
                return;
        }
        usleep(1);
    }
}
