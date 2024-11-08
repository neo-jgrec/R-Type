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
    EventFactory::playerProjectileShoot(*this);
    EventFactory::playerMissileShoot(*this);

    _gameEngine.addCommand("stop", "Stop the server", [this](const std::string &) {
        _gameState = STOPPING;
        return "Server stopping";
    });
    _gameEngine.addCommand("kick", "Kick a player", [this](const std::string &args) {
        if (args.empty())
            return "Usage: kick <player_id>";
        const uint8_t id = std::stoi(args);
        if (id >= 4 || !_playersConnection[id].has_value())
            return "Player not found";
        _playersConnection[id].reset();
        _gameEngine.registry.kill_entity(_players[id].value());
        _players[id].reset();
        return "Player kicked";
    });
}

void Server::start()
{
    static bool asGameStarted = false;
    if (asGameStarted)
        return;
    asGameStarted = true;

    *_gameEngine.out << "Game starting" << std::endl;

    const core::ecs::Entity world = EntityFactory::createWorld(*this, "assets/JY_map.json");
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
                *_gameEngine.out << "Server started" << std::endl;
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
                *_gameEngine.out << "Server stopped" << std::endl;
                _networkingService.stop();
                return;
        }
        #ifdef __linux__
            usleep(1);
        #elif _WIN32
            _sleep(1);
        #endif
    }
}
