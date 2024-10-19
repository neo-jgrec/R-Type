#include "Server.hpp"
#include "Components.hpp"
#include "EntityFactory.hpp"
#include "Systems.hpp"
#include "EventFactory.hpp"
#include "../../../game/RequestType.hpp"

bool Server::asPlayerConnected()
{
    return std::ranges::any_of(_players, [](const auto &entity) { return entity.has_value(); });
}

void Server::sendRequestToPlayers(const uint8_t requestType, const std::vector<uint8_t> &payload)
{
    for (const auto &playerEntity : _players) {
        if (!playerEntity.has_value())
            continue;
        const auto &playerComponent = _gameEngine.registry.get_component<Player>(playerEntity.value());
        _networkingService.sendRequest(
            *playerComponent->endpoint,
            requestType,
            payload);
    }
}

void Server::sendRequestToPlayers(const uint8_t requestType, const std::vector<uint8_t> &payload, uint8_t selfPlayer)
{
    for (const auto &playerEntity : _players) {
        if (!playerEntity.has_value())
            continue;
        const auto &playerComponent = _gameEngine.registry.get_component<Player>(playerEntity.value());
        if (playerComponent->id == selfPlayer)
            continue;
        _networkingService.sendRequest(
            *playerComponent->endpoint,
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

    Systems::worldSystem(*this);
    Systems::enemySystem(*this);

    EventFactory::gameStarted(*this);
    EventFactory::playerConnected(*this);
    EventFactory::playerDisconnected(*this);
}

void Server::start()
{
    static bool asGameStarted = false;
    if (!asPlayerConnected() || asGameStarted)
        return;

    asGameStarted = true;
    const core::ecs::Entity world = EntityFactory::createWorld(*this, "JY_map.json");
    for (const auto &playerEntity : _gameEngine.registry.get_entities<Player>()) {
        const auto &playerComponent = _gameEngine.registry.get_component<Player>(playerEntity);
        _networkingService.sendRequest(
            *playerComponent->endpoint,
            GameStart,
            {});
        const auto &worldComponent = _gameEngine.registry.get_component<World>(world);
        const auto scroll = static_cast<uint32_t>(worldComponent->scroll);
        _networkingService.sendRequest(
            *playerComponent->endpoint,
            MapScroll,
            {
                static_cast<uint8_t>(scroll >> 24),
                static_cast<uint8_t>(scroll >> 16),
                static_cast<uint8_t>(scroll >> 8),
                static_cast<uint8_t>(scroll),
            });
        const std::vector payload = {
            playerComponent->id,
            static_cast<uint8_t>(100 >> 24),
            static_cast<uint8_t>(100 >> 16),
            static_cast<uint8_t>(100 >> 8),
            static_cast<uint8_t>(100),
            static_cast<uint8_t>(100 >> 24),
            static_cast<uint8_t>(100 >> 16),
            static_cast<uint8_t>(100 >> 8),
            static_cast<uint8_t>(100),
        };
        _networkingService.sendRequest(
            *playerComponent->endpoint,
            PlayerMove,
            payload);
        playerComponent->lastTimePacketReceived = std::time(nullptr);
        playerComponent->health = 3;
        _asGameStarted = true;
    }
}

void Server::update()
{
    std::lock_guard lock(registry_mutex);

    _gameEngine.registry.run_system<World>();
    _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::VelocityComponent>();
    _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::CollisionComponent>();
    _gameEngine.registry.run_system<core::ge::TransformComponent, Enemy>();
}

void Server::run()
{
    _networkingService.run();

    std::cout << "Server started" << std::endl;
    while (!asPlayerConnected()) {
        usleep(1);
    }
    std::cout << "Players connected" << std::endl;
    while (!_asGameStarted) {
        usleep(1);
    }
    EventFactory::playerMove(*this);
    EventFactory::playerShoot(*this);
    std::cout << "Game started" << std::endl;
    while (asPlayerConnected()) {
        sf::Time elapsed = _gameEngine.clock.restart();
        _gameEngine.delta_t = elapsed.asSeconds();

        update();
        usleep(1);
    }

    std::cout << "Server stopped" << std::endl;
    _networkingService.stop();
}
