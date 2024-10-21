#include "EntityFactory.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>

#include "Components.hpp"
#include "Server.hpp"
#include "../../../game/CollisionMask.hpp"
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"
#include "../../../game/RequestType.hpp"

core::ecs::Entity EntityFactory::createWorld(
    Server &server,
    const std::string& filePath)
{
    core::GameEngine &gameEngine = server.getGameEngine();
    NetworkingService &networkingService = server.getNetworkingService();

    const core::ecs::Entity world = gameEngine.registry.spawn_entity();

    gameEngine.registry.add_component(world, Network{networkingService});
    gameEngine.registry.add_component(world, core::ge::TransformComponent{sf::Vector2f(0, 0), sf::Vector2f(1920, 1080), sf::Vector2f(1, 1), 0});
    gameEngine.registry.add_component(world, core::ge::CollisionComponent{WORLD, {
        {-200, -100, 100, 100},
        {2120, -100, 100, 100}
    }});

    nlohmann::json json;
    {
        std::ifstream file(filePath);
        if (!file.is_open())
            throw std::runtime_error("Cannot open file: " + filePath);
        file >> json;
    }

    World worldComponent = {75, 0, { json["width"], json["height"] }, {}};
    const uint8_t tileSize = json["cellSize"];
    for (const auto& tile : json["tiles"]) {
        if (tile.contains("tags")) {
            if (std::vector<std::string> tags = tile["tags"]; tags.end() == std::ranges::find(tags, "spawn"))
                continue;

            const uint32_t x = tile["x"];
            const uint32_t y = tile["y"];
            worldComponent.spawnPoints.emplace_back(x * tileSize, y * tileSize);
            continue;
        }

        if (tile["x"] < 0 || tile["x"] >= worldComponent.size.first
            || tile["y"] < 0 || tile["y"] >= worldComponent.size.second) {
            throw std::out_of_range("Tile coordinates out of bounds");
        }

        const uint32_t x = tile["x"];
        const uint32_t y = tile["y"];
        createTile(server, {tileSize, tileSize}, x * tileSize, y * tileSize);
    }
    gameEngine.registry.add_component(world, std::move(worldComponent));

    return world;
}

core::ecs::Entity EntityFactory::createPlayer(
    Server &server,
    const uint8_t id)
{
    core::GameEngine &gameEngine = server.getGameEngine();
    NetworkingService &networkingService = server.getNetworkingService();
    const auto &playersConnection = server.getPlayersConnection();
    auto &players = server.getPlayers();

    const core::ecs::Entity world = gameEngine.registry.get_entities<World>()[0];
    const auto &worldComponent = gameEngine.registry.get_component<World>(world);

    const std::function onCollision = [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
        std::cout << "Player " << static_cast<int>(id) << " collided" << std::endl;

        RequestType requestType = PlayerHit;
        {
            const auto &playerComponent = gameEngine.registry.get_component<Player>(entity);
            playerComponent->health -= 1;
            if (playerComponent->health == 0)
                requestType = PlayerDie;
        }

        server.sendRequestToPlayers(requestType, {id});

        if (requestType == PlayerDie) {
            std::cout << "Player " << static_cast<int>(id) << " died" << std::endl;
            players[id].reset();
            gameEngine.registry.kill_entity(entity);
        }

        if (std::ranges::none_of(gameEngine.registry.get_entities<Player>(), [&](const auto &playerEntity) {
            const auto &playerComponent = gameEngine.registry.get_component<Player>(playerEntity);
            return playerComponent->health > 0;
        })) {
            server.sendRequestToPlayers(GameOver, {});
        }
    };

    const core::ecs::Entity player = gameEngine.registry.spawn_entity();
    const auto [fst, snd] = worldComponent->spawnPoints[rand() % worldComponent->spawnPoints.size()];
    const core::ge::TransformComponent transformComponent = {sf::Vector2f(static_cast<float>(fst), static_cast<float>(snd)), sf::Vector2f(32, 32), sf::Vector2f(1, 1), 0};
    const Player playerComponent = {id, 3, std::time(nullptr)};

    gameEngine.registry.add_component(player, Network{networkingService});
    gameEngine.registry.add_component(player, std::move(transformComponent));
    gameEngine.registry.add_component(player, core::ge::CollisionComponent{PLAYER, std::vector{sf::FloatRect(0, 0, 32, 32)},{
        {ENEMY, onCollision},
        {TILE, onCollision}}});
    gameEngine.registry.add_component(player, std::move(playerComponent));

    std::cout << ">>" << std::endl;
    networkingService.sendRequest(
        *playersConnection[id].value(),
        GameStart,
        {});
    std::cout << "<<" << std::endl;
    {
        const auto scroll = static_cast<uint32_t>(worldComponent->scroll);
        networkingService.sendRequest(
            *playersConnection[id].value(),
            MapScroll,
            {
                static_cast<uint8_t>(scroll >> 24),
                static_cast<uint8_t>(scroll >> 16),
                static_cast<uint8_t>(scroll >> 8),
                static_cast<uint8_t>(scroll),
            });
    }
    {
        const auto x = static_cast<uint32_t>(fst);
        const auto y = static_cast<uint32_t>(snd);

        server.sendRequestToPlayers(PlayerMove, {
            id,
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

    std::cout << "Player " << static_cast<int>(id) << " created" << std::endl;
    return player;
}

core::ecs::Entity EntityFactory::createEnemy(Server &server, const uint32_t x)
{
    static uint8_t id = 0;
    if (id >= 255)
        return core::ecs::Entity{};

    core::GameEngine &gameEngine = server.getGameEngine();
    NetworkingService &networkingService = server.getNetworkingService();
    const auto &playersConnection = server.getPlayersConnection();

    const core::ecs::Entity enemy = gameEngine.registry.spawn_entity();

    const sf::Vector2i position = {static_cast<int>(x), rand() % 880 + 100};
    gameEngine.registry.add_component(enemy, Network{networkingService});
    gameEngine.registry.add_component(enemy, core::ge::TransformComponent{sf::Vector2f(position), sf::Vector2f(32, 32), sf::Vector2f(1, 1), 0});
    gameEngine.registry.add_component(enemy, core::ge::VelocityComponent{-100, 0});
    gameEngine.registry.add_component(enemy, core::ge::CollisionComponent{ENEMY, std::vector{sf::FloatRect(0, 0, 32, 32)},{
        { PLAYER, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Enemy " << static_cast<int>(id) << " collided with player" << std::endl;
            server.sendRequestToPlayers(EnemyDie, {id});
            gameEngine.registry.kill_entity(entity);
        }},
        { TILE, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Enemy " << static_cast<int>(id) << " collided with tile" << std::endl;
            server.sendRequestToPlayers(EnemyDie, {id});
            gameEngine.registry.kill_entity(entity);
        }},
        { WORLD, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Enemy " << static_cast<int>(id) << " collided with world" << std::endl;
            server.sendRequestToPlayers(EnemyDie, {id});
            gameEngine.registry.kill_entity(entity);
        }}}});
    gameEngine.registry.add_component(enemy, Enemy{id});

    const std::vector payload = {
        id,
        static_cast<uint8_t>(position.x >> 24),
        static_cast<uint8_t>(position.x >> 16),
        static_cast<uint8_t>(position.x >> 8),
        static_cast<uint8_t>(position.x),
        static_cast<uint8_t>(position.y >> 24),
        static_cast<uint8_t>(position.y >> 16),
        static_cast<uint8_t>(position.y >> 8),
        static_cast<uint8_t>(position.y)
    };
    for (auto &playerEntity : gameEngine.registry.get_entities<Player>()) {
        const auto &playerComponent = gameEngine.registry.get_component<Player>(playerEntity);
        networkingService.sendRequest(
            *playersConnection[playerComponent->id].value(),
            EnemySpawn,
            payload);
    }

    std::cout << "Enemy " << static_cast<int>(id++) << " created" << std::endl;
    return enemy;
}

core::ecs::Entity EntityFactory::createProjectile(
    Server &server,
    const core::ecs::Entity &player,
    const uint8_t id)
{
    core::GameEngine &gameEngine = server.getGameEngine();

    const core::ecs::Entity projectile = gameEngine.registry.spawn_entity();

    const auto &playerTransform = gameEngine.registry.get_component<core::ge::TransformComponent>(player);

    gameEngine.registry.add_component(projectile, core::ge::TransformComponent{playerTransform->position, sf::Vector2f(8, 8), sf::Vector2f(1, 1), 0});
    gameEngine.registry.add_component(projectile, core::ge::VelocityComponent{200, 0});
    gameEngine.registry.add_component(projectile, core::ge::CollisionComponent{PLAYER_PROJECTILE, std::vector{sf::FloatRect(0, 0, 8, 8)},{
        { ENEMY, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Projectile collided with enemy" << std::endl;
            gameEngine.registry.kill_entity(entity);
        }},
        { WORLD, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Projectile collided with world" << std::endl;
            gameEngine.registry.kill_entity(entity);
        }},
        { TILE, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Projectile collided with tile" << std::endl;
            gameEngine.registry.kill_entity(entity);
        }}}});
    gameEngine.registry.add_component(projectile, Projectile{id});

    std::cout << "Projectile created" << std::endl;
    return projectile;
}

core::ecs::Entity EntityFactory::createTile(
    Server &server,
    const std::pair<uint32_t, uint32_t> &size,
    const uint32_t x,
    const uint32_t y)
{
    core::GameEngine &gameEngine = server.getGameEngine();
    NetworkingService &networkingService = server.getNetworkingService();
    const auto &playersConnection = server.getPlayersConnection();

    const core::ecs::Entity tile = gameEngine.registry.spawn_entity();

    gameEngine.registry.add_component(tile, core::ge::TransformComponent{sf::Vector2f(static_cast<float>(x * size.first), static_cast<float>(y * size.second)), sf::Vector2f(static_cast<float>(size.first), static_cast<float>(size.second)), sf::Vector2f(1, 1), 0});
    gameEngine.registry.add_component(tile, core::ge::CollisionComponent{TILE, std::vector{sf::FloatRect(0, 0, static_cast<float>(size.first), static_cast<float>(size.second))}, {
        { PLAYER_PROJECTILE, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Tile collided with projectile" << std::endl;

            for (auto &playerEntity : gameEngine.registry.get_entities<Player>()) {
                const auto &playerComponent = gameEngine.registry.get_component<Player>(playerEntity);
                networkingService.sendRequest(
                    *playersConnection[playerComponent->id].value(),
                    TileDestroy,
                    {playerComponent->id});
            }

            gameEngine.registry.kill_entity(entity);
        }}}});

    return tile;
}
