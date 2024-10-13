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

    Systems::worldSystem(_gameEngine.registry, _players);
    Systems::playerSystem(_gameEngine.registry, _players);
    Systems::enemySystem(_gameEngine.registry, _players);
    Systems::projectileSystem(_gameEngine.registry);

    _networkingService.addEvent(PlayerConnect, [&](const GDTPHeader &header, const std::vector<uint8_t> &, const asio::ip::udp::endpoint &endpoint) {
        std::cout << "New connection from " << endpoint << std::endl;
        for (uint8_t i = 0; i < 4; i++) {
            if (_players[i].has_value())
                continue;
            _players[i].emplace(EntityFactory::createPlayer(_gameEngine.registry, _networkingService, _players, endpoint, i));
            _networkingService.sendRequestResponse(endpoint, header, {i});
            return;
        }
    });
    _networkingService.addEvent(GameStart, [&](const GDTPHeader &, const std::vector<uint8_t> &, const asio::ip::udp::endpoint &) {
        if (!asPlayerConnected())
            return;
        std::cout << "Game started" << std::endl;
        _world = EntityFactory::createWorld(_gameEngine.registry, _networkingService, _players, "JY_map.json");
        for (uint8_t i = 0; i < 4; i++) {
            if (!_players[i].has_value())
                continue;
            const auto &playerComponent = _gameEngine.registry.get_component<Player>(_players[i].value());
            _networkingService.sendRequest(
                playerComponent->endpoint,
                GameStart,
                {});
            _networkingService.sendRequest(
                playerComponent->endpoint,
                MapScroll,
                {0, 0, 0, 0});
            _networkingService.sendRequest(
                playerComponent->endpoint,
                PlayerMove,
                {playerComponent->id, 0, 0, 0, 0, 0, 0, 0, 0});
            playerComponent->lastTimePacketReceived = std::time(nullptr);
            playerComponent->health = 3;
        }
        _asGameStarted = true;
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
    std::cout << "↻ Updating server" << std::endl;
    _gameEngine.registry.run_system<Network, World>();
    _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::CollisionComponent>();
    // _gameEngine.registry.run_system<Network, Player>();
    _gameEngine.registry.run_system<Network, Enemy>();
    _gameEngine.registry.run_system<Projectile>();
}

bool Server::asPlayerConnected()
{
    return std::ranges::any_of(_players, [](const auto &entity) { return entity.has_value(); });
}

void Server::run()
{
    _networkingService.run();

    std::cout << "Server started" << std::endl;
    while (!asPlayerConnected()) {
        #ifdef _WIN32
            _sleep(1000);
        #else
            sleep(1);
        #endif
    }
    std::cout << "Players connected" << std::endl;
    while (!_asGameStarted) {
        #ifdef _WIN32
            _sleep(1000);
        #else
            sleep(1);
        #endif
    }
    _networkingService.addEvent(PlayerMove, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        if (!asPlayerConnected())
            return;
        const uint8_t id = payload[0];
        if (id >= 4 || !_players[id].has_value())
            return;

        {
            const auto &transformComponent = _gameEngine.registry.get_component<core::ge::TransformComponent>(_players[id].value());
            const uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
            const uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
            transformComponent->position = {static_cast<float>(x), static_cast<float>(y)};
        }

        for (const auto &playerEntity : _players) {
            if (!playerEntity.has_value())
                continue;
            const auto &playerComponent = _gameEngine.registry.get_component<Player>(playerEntity.value());
            std::cout << "Sending player move to " << static_cast<int>(playerComponent->id) << std::endl;
            _networkingService.sendRequest(
                playerComponent->endpoint,
                PlayerMove,
                payload);
            if (playerComponent->id == id)
                playerComponent->lastTimePacketReceived = std::time(nullptr);
        }
    });
    _networkingService.addEvent(PlayerShoot, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &)
    {
        if (!asPlayerConnected())
            return;
        const uint8_t id = payload[0];
        if (id >= 4 || !_players[id].has_value())
            return;

        static uint8_t projectileId = 0;
        EntityFactory::createProjectile(_gameEngine.registry, _players[id].value(), projectileId++);
        std::cout << "Player " << static_cast<int>(id) << " shot" << std::endl;

        for (const auto &playerEntity : _players) {
            if (!playerEntity.has_value())
                continue;
            const auto &playerComponent = _gameEngine.registry.get_component<Player>(playerEntity.value());
            _networkingService.sendRequest(
                playerComponent->endpoint,
                PlayerShoot,
                payload);
        }
    });
    std::cout << "Game started" << std::endl;
    while (asPlayerConnected()) {
        update();
        #ifdef _WIN32
            _sleep(1000);
        #else
            sleep(1);
        #endif
    }

    std::cout << "Server stopped" << std::endl;
    _networkingService.stop();
}
