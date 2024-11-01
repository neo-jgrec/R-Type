#include "EventFactory.hpp"

#include "Components.hpp"
#include "EntityFactory.hpp"
#include "../../../game/RequestType.hpp"

void EventFactory::gameStarted(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();

    networkingService.addEvent(GameStart, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        if (!payload.empty())
            return;

        std::lock_guard lock(server.getRegistryMutex());

        server.start();
    });
}

void EventFactory::playerConnected(Server &server)
{
    NetworkingService &networkingService = server.getNetworkingService();
    auto &playersConnection = server.getPlayersConnection();
    auto &players = server.getPlayers();

    networkingService.addEvent(PlayerConnect, [&](const GDTPHeader &header, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &endpoint) {
        if (!payload.empty())
            return;

        std::lock_guard lock(server.getRegistryMutex());

        uint8_t id = 0;
        for (; id < 4; id++) {
            if (playersConnection[id].has_value())
                continue;
            std::cout << "New connection from " << endpoint << std::endl;
            playersConnection[id] = std::make_shared<asio::ip::udp::endpoint>(endpoint);
            break;
        }
        if (id == 4)
            return;

        // send the new player to himself
        networkingService.sendRequestResponse(endpoint, header, {id});

        for (uint8_t i = 0; i < 4; i++) {
            if (!playersConnection[i].has_value() || i == id)
                continue;
            // send all players to the new player
            networkingService.sendRequest(
                endpoint,
                PlayerConnect,
                {i});
            // send the new player to all players
            networkingService.sendRequest(
                *playersConnection[i].value(),
                PlayerConnect,
                {i});
        }

        if (server.getGameState() != GAME)
            return;

        players[id] = EntityFactory::createPlayer(server, id);

        // Send all player positions to the new player
        for (uint8_t i = 0; i < 4; i++) {
            if (!players[i].has_value() || i == id)
                continue;
            const auto &transformComponent = server.getGameEngine().registry.get_component<core::ge::TransformComponent>(players[i].value());
            const auto x = static_cast<uint32_t>(transformComponent->position.x);
            const auto y = static_cast<uint32_t>(transformComponent->position.y);

            for (uint8_t j = 0; j < 4; j++) {
                if (!playersConnection[j].has_value() || j == id)
                    continue;
                networkingService.sendRequest(
                    endpoint,
                    PlayerMove,
                    {
                        i,
                        static_cast<uint8_t>(x >> 24),
                        static_cast<uint8_t>(x >> 16),
                        static_cast<uint8_t>(x >> 8),
                        static_cast<uint8_t>(x),
                        static_cast<uint8_t>(y >> 24),
                        static_cast<uint8_t>(y >> 16),
                        static_cast<uint8_t>(y >> 8),
                        static_cast<uint8_t>(y)
                    });
            }
        }

        // Send all enemies to the new player
        for (const auto &enemy : server.getGameEngine().registry.get_entities<Enemy>()) {
            const auto enemyComponent = server.getGameEngine().registry.get_component<Enemy>(enemy);
            const auto &transformComponent = server.getGameEngine().registry.get_component<core::ge::TransformComponent>(enemy);
            const auto x = static_cast<uint32_t>(transformComponent->position.x);
            const auto y = static_cast<uint32_t>(transformComponent->position.y);

            networkingService.sendRequest(
                endpoint,
                EnemySpawn,
                {
                    enemyComponent->id,
                    static_cast<uint8_t>(x >> 24),
                    static_cast<uint8_t>(x >> 16),
                    static_cast<uint8_t>(x >> 8),
                    static_cast<uint8_t>(x),
                    static_cast<uint8_t>(y >> 24),
                    static_cast<uint8_t>(y >> 16),
                    static_cast<uint8_t>(y >> 8),
                    static_cast<uint8_t>(y)
                });
        }
    });
}

void EventFactory::playerDisconnected(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();
    core::GameEngine &gameEngine = server.getGameEngine();
    auto &playersConnection = server.getPlayersConnection();
    auto &players = server.getPlayers();

    networkingService.addEvent(PlayerDisconnect, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        if (payload.size() != 1)
            return;

        std::lock_guard lock(server.getRegistryMutex());

        const uint8_t id = payload[0];
        if (id >= 4 || !playersConnection[id].has_value())
            return;

        std::cout << "Player " << static_cast<int>(id) << " disconnected" << std::endl;
        playersConnection[id].reset();

        if (!players[id].has_value())
            return;
        gameEngine.registry.kill_entity(players[id].value());
        players[id].reset();

        if (server.asPlayerConnected())
            return;
        server.setGameState(STOPPING);
    });
}

void EventFactory::playerMove(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();
    core::GameEngine &gameEngine = server.getGameEngine();
    const auto &players = server.getPlayers();

    networkingService.addEvent(PlayerMove, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        if (payload.size() != 9)
            return;

        std::lock_guard lock(server.getRegistryMutex());

        const uint8_t id = payload[0];
        if (id >= 4 || !players[id].has_value())
            return;

        {
            const auto &transformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(players[id].value());
            transformComponent->position = {
                static_cast<float>((payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4]),
                static_cast<float>((payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8])
            };
        }

        {
            const auto &playerComponent = gameEngine.registry.get_component<Player>(players[id].value());
            playerComponent->lastTimePacketReceived = std::time(nullptr);
        }

        server.sendRequestToPlayers(PlayerMove, payload, id);
    });
}

void EventFactory::playerProjectileShoot(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();
    core::GameEngine &gameEngine = server.getGameEngine();
    const std::array<std::optional<core::ecs::Entity>, 4> &players = server.getPlayers();

    networkingService.addEvent(PlayerProjectileShoot, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        if (payload.size() != 1)
            return;

        std::lock_guard lock(server.getRegistryMutex());

        const uint8_t id = payload[0];
        if (id >= 4 || !players[id].has_value())
            return;

        EntityFactory::createProjectile(server, players[id].value());

        const auto &playerComponent = gameEngine.registry.get_component<Player>(players[id].value());
        playerComponent->lastTimePacketReceived = std::time(nullptr);
    });
}

void EventFactory::playerMissileShoot(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();
    core::GameEngine &gameEngine = server.getGameEngine();
    const std::array<std::optional<core::ecs::Entity>, 4> &players = server.getPlayers();

    networkingService.addEvent(PlayerMissileShoot, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        if (payload.size() != 1)
            return;

        std::lock_guard lock(server.getRegistryMutex());

        const uint8_t id = payload[0];
        if (id >= 4 || !players[id].has_value())
            return;

        EntityFactory::createMissile(server, players[id].value());

        const auto &playerComponent = gameEngine.registry.get_component<Player>(players[id].value());
        playerComponent->lastTimePacketReceived = std::time(nullptr);
    });
}
