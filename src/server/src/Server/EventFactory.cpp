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
        // send the new player to himself
        networkingService.sendRequestResponse(endpoint, header, {id});
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

void EventFactory::playerShoot(Server &server)
{
    const NetworkingService &networkingService = server.getNetworkingService();
    core::GameEngine &gameEngine = server.getGameEngine();
    const std::array<std::optional<core::ecs::Entity>, 4> &players = server.getPlayers();

    networkingService.addEvent(PlayerShoot, [&](const GDTPHeader &, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &) {
        if (payload.size() != 1)
            return;

        std::lock_guard lock(server.getRegistryMutex());

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
