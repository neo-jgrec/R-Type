#include "Server.hpp"
#include "Components.hpp"
#include "Systems.hpp"
#include "EntityFactory.hpp"
#include "../../../game/RequestType.hpp"

Server::Server()
{
    _gameEngine.registry.register_component<Network>();
    _gameEngine.registry.register_component<World>();
    _gameEngine.registry.register_component<Player>();
    _gameEngine.registry.register_component<Enemy>();
    _gameEngine.registry.register_component<Projectile>();

    Systems::worldSystem(*this);
    Systems::enemySystem(*this);

    _networkingService.addEvent(PlayerConnect, [&](const GDTPHeader &header, const std::vector<uint8_t> &, const asio::ip::udp::endpoint &endpoint) {
        std::cout << "New connection from " << endpoint << std::endl;

        uint8_t i = 0;
        for (; i < 4; i++) {
            if (_players[i].has_value()) {
                continue;
            }
            _players[i].emplace(EntityFactory::createPlayer(*this, endpoint, i));
            _networkingService.sendRequestResponse(endpoint, header, {i});
            break;
        }
        if (i == 4)
            return;

        for (const auto &playerEntity : _players) {
            if (!playerEntity.has_value())
                continue;
            const auto &playerComponent = _gameEngine.registry.get_component<Player>(playerEntity.value());
            // send all players to the new player
            if (playerComponent->id != i)
                _networkingService.sendRequest(
                    endpoint,
                    PlayerConnect,
                    {playerComponent->id});
            // send the new player to all players
            _networkingService.sendRequest(
                *playerComponent->endpoint,
                PlayerConnect,
                {i});
        }
    });
    _networkingService.addEvent(GameStart, [&](const GDTPHeader &, const std::vector<uint8_t> &, const asio::ip::udp::endpoint &) {
        if (!asPlayerConnected() || _asGameStarted)
            return;

        _asGameStarted = true;
        const core::ecs::Entity world = EntityFactory::createWorld(*this, "JY_map.json");
        for (uint8_t i = 0; i < 4; i++) {
            if (!_players[i].has_value())
                continue;
            const auto &playerComponent = _gameEngine.registry.get_component<Player>(_players[i].value());
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
        }
    });
    _networkingService.addEvent(PlayerDisconnect, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &)
    {
        const uint8_t id = payload[0];
        if (id >= 4 || !_players[id].has_value())
            return;

        std::cout << "Player " << static_cast<int>(id) << " disconnected" << std::endl;
        _gameEngine.registry.kill_entity(_players[id].value());
        _players[id].reset();
    });
}

void Server::update()
{
    // _gameEngine.registry.run_system<World>();
    // _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::VelocityComponent>();
    // _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::CollisionComponent>();
    // _gameEngine.registry.run_system<core::ge::TransformComponent, Enemy>();
}

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

void Server::run()
{
    _networkingService.run();

    std::cout << "Server started" << std::endl;
    while (!asPlayerConnected()) {}
    std::cout << "Players connected" << std::endl;
    while (!_asGameStarted) {}
    _networkingService.addEvent(PlayerMove, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        if (!asPlayerConnected())
            return;
        const uint8_t id = payload[0];
        std::cout << "Player " << static_cast<int>(id) << " moved" << std::endl;
        if (id >= 4 || !_players[id].has_value())
            return;

        {
            const auto &transformComponent = _gameEngine.registry.get_component<core::ge::TransformComponent>(_players[id].value());
            const uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
            const uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
            transformComponent->position = {static_cast<float>(x), static_cast<float>(y)};
        }

        _gameEngine.registry.get_component<Player>(_players[id].value())->lastTimePacketReceived = std::time(nullptr);
        sendRequestToPlayers(PlayerMove, payload);
    });
    _networkingService.addEvent(PlayerShoot, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &)
    {
        if (!asPlayerConnected())
            return;
        const uint8_t id = payload[0];
        if (id >= 4 || !_players[id].has_value())
            return;

        static uint8_t projectileId = 0;
        EntityFactory::createProjectile(*this, _players[id].value(), projectileId++);
        std::cout << "Player " << static_cast<int>(id) << " shot" << std::endl;

        sendRequestToPlayers(PlayerShoot, payload);
    });
    std::cout << "Game started" << std::endl;
    while (asPlayerConnected()) {
        sf::Time elapsed = _gameEngine.clock.restart();
        _gameEngine.delta_t = elapsed.asSeconds();

        for (auto &playerEntity : _gameEngine.registry.get_entities<Player>()) {
            // Crash here
            _gameEngine.registry.get_component<core::ge::TransformComponent>(playerEntity);
        }

        update();
    }

    std::cout << "Server stopped" << std::endl;
    _networkingService.stop();
}
