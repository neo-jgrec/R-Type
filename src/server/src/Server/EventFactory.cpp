#include "EventFactory.hpp"

#include "Components.hpp"
#include "EntityFactory.hpp"
#include "../../../game/RequestType.hpp"

void EventFactory::gameStarted(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();

    networkingService.addEvent(GameStart, [&](const GDTPHeader &, const std::vector<uint8_t> &, const asio::ip::udp::endpoint &) {
        std::lock_guard lock(server.getRegistryMutex());

        server.start();
    });
}

void EventFactory::playerConnected(Server &server)
{
    NetworkingService &networkingService = server.getNetworkingService();
    core::GameEngine &gameEngine = server.getGameEngine();
    std::array<std::optional<core::ecs::Entity>, 4> &players = server.getPlayers();

    networkingService.addEvent(PlayerConnect, [&](const GDTPHeader &header, const std::vector<uint8_t> &, const asio::ip::udp::endpoint &endpoint) {
        std::cout << "New connection from " << endpoint << std::endl;

        uint8_t i = 0;
        for (; i < 4; i++)
            if (!players[i].has_value())
                break;
        if (i == 4)
            return;

        // const auto &worldEntity = gameEngine.registry.get_entities<World>()[0];

        players[i].emplace(EntityFactory::createPlayer(server, endpoint, i));
        for (const auto &playerEntity : gameEngine.registry.get_entities<Player>()) {
            const auto &playerComponent = gameEngine.registry.get_component<Player>(playerEntity);
            if (playerComponent->id == i)
                continue;
            // send all players to the new player
            networkingService.sendRequest(
                endpoint,
                PlayerConnect,
                {playerComponent->id});
            // send the new player to all players
            networkingService.sendRequest(
                *playerComponent->endpoint,
                PlayerConnect,
                {i});
        }
        // send the new player to himself
        networkingService.sendRequestResponse(endpoint, header, {i});
        if (!server.asGameStarted())
            return;

        networkingService.sendRequest(
            endpoint,
            GameStart,
            {});

        {
            const core::ecs::Entity world = gameEngine.registry.get_entities<World>()[0];
            const auto &worldComponent = gameEngine.registry.get_component<World>(world);

            {
                const auto scroll = static_cast<uint32_t>(worldComponent->scroll);
                networkingService.sendRequest(
                    endpoint,
                    MapScroll,
                    {
                        static_cast<uint8_t>(scroll >> 24),
                        static_cast<uint8_t>(scroll >> 16),
                        static_cast<uint8_t>(scroll >> 8),
                        static_cast<uint8_t>(scroll)
                    });
            }
            {
                const auto [fst, snd] = worldComponent->spawnPoints[rand() % worldComponent->spawnPoints.size()];
                const auto x = static_cast<uint32_t>(fst);
                const auto y = static_cast<uint32_t>(snd);

                const auto playerComponent = gameEngine.registry.get_component<Player>(players[i].value());
                const auto transformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(players[i].value());
                transformComponent->position = {static_cast<float>(x), static_cast<float>(y)};

                server.sendRequestToPlayers(PlayerMove, {
                    playerComponent->id,
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

        for (const auto &playerEntity : gameEngine.registry.get_entities<Player>()) {
            const auto &playerComponent = gameEngine.registry.get_component<Player>(playerEntity);
            if (playerComponent->id == i)
                continue;
            const auto &transformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(playerEntity);
            const auto x = static_cast<uint32_t>(transformComponent->position.x);
            const auto y = static_cast<uint32_t>(transformComponent->position.y);
            const std::vector payload = {
                playerComponent->id,
                static_cast<uint8_t>(x >> 24),
                static_cast<uint8_t>(x >> 16),
                static_cast<uint8_t>(x >> 8),
                static_cast<uint8_t>(x),
                static_cast<uint8_t>(y >> 24),
                static_cast<uint8_t>(y >> 16),
                static_cast<uint8_t>(y >> 8),
                static_cast<uint8_t>(y)
            };
            networkingService.sendRequest(
                endpoint,
                PlayerMove,
                payload);
        }
    });
}

void EventFactory::playerDisconnected(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();
    core::GameEngine &gameEngine = server.getGameEngine();
    std::array<std::optional<core::ecs::Entity>, 4> &players = server.getPlayers();

    networkingService.addEvent(PlayerDisconnect, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        const uint8_t id = payload[0];
        if (id >= 4 || !players[id].has_value())
            return;

        std::cout << "Player " << static_cast<int>(id) << " disconnected" << std::endl;
        gameEngine.registry.kill_entity(players[id].value());
        players[id].reset();
    });
}

void EventFactory::playerMove(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();
    core::GameEngine &gameEngine = server.getGameEngine();
    const std::array<std::optional<core::ecs::Entity>, 4> &players = server.getPlayers();

    networkingService.addEvent(PlayerMove, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        const uint8_t id = payload[0];
        if (id >= 4 || !players[id].has_value())
            return;

        {
            const auto &transformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(players[id].value());
            const uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
            const uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
            transformComponent->position = {static_cast<float>(x), static_cast<float>(y)};
        }

        {
            const auto &playerComponent = gameEngine.registry.get_component<Player>(players[id].value());
            playerComponent->lastTimePacketReceived = std::time(nullptr);
        }

        server.sendRequestToPlayers(PlayerMove, payload, id);
    });
}

void EventFactory::playerShoot(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();
    core::GameEngine &gameEngine = server.getGameEngine();
    const std::array<std::optional<core::ecs::Entity>, 4> &players = server.getPlayers();

    networkingService.addEvent(PlayerShoot, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        const uint8_t id = payload[0];
        if (id >= 4 || !players[id].has_value())
            return;

        static uint8_t projectileId = 0;
        EntityFactory::createProjectile(server, players[id].value(), projectileId++);

        {
            const auto &playerComponent = gameEngine.registry.get_component<Player>(players[id].value());
            playerComponent->lastTimePacketReceived = std::time(nullptr);
        }

        server.sendRequestToPlayers(PlayerShoot, payload);
    });
}
