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
    auto &gameEngine = server.getGameEngine();
    auto &networkingService = server.getNetworkingService();
    auto &config = server.getConfigManager();

    const core::ecs::Entity world = gameEngine.registry.spawn_entity();

    const sf::Vector2f size = {config.getValue<float>("/view/size/x", 1920.0f), config.getValue<float>("/view/size/y", 1080.0f)};

    gameEngine.registry.add_component(world, Network{networkingService});
    gameEngine.registry.add_component(world, core::ge::VelocityComponent{
        config.getValue<float>("/view/speed/x", 50.0f),
        config.getValue<float>("/view/speed/y", 0)});
    gameEngine.registry.add_component(world, core::ge::TransformComponent{sf::Vector2f(0, 0), size, sf::Vector2f(1, 1), 0});
    gameEngine.registry.add_component(world, core::ge::CollisionComponent{WORLD, {
        {-1000, 0, 1000, 1080},
        {2250, 0, 1000, 1080}
    }});

    nlohmann::json json;
    {
        std::ifstream file(filePath);
        if (!file.is_open())
            throw std::runtime_error("Cannot open file: " + filePath);
        file >> json;
    }

    World worldComponent = {
        std::time(nullptr), 1,
        { size.x, size.y }, json["cellSize"], {}};
    for (const auto& tile : json["tiles"]) {
        if (tile.contains("tags")) {
            if (std::vector<std::string> tags = tile["tags"]; tags.end() == std::ranges::find(tags, "spawn"))
                continue;

            const uint32_t x = tile["x"];
            const uint32_t y = tile["y"];
            worldComponent.spawnPoints.emplace_back(x * worldComponent.tileSize, y * worldComponent.tileSize);
            continue;
        }

        if (tile["x"] < 0 || tile["x"] >= worldComponent.size.first
            || tile["y"] < 0 || tile["y"] >= worldComponent.size.second) {
            throw std::out_of_range("Tile coordinates out of bounds");
        }

        const uint32_t x = tile["x"];
        const uint32_t y = tile["y"];
        createTile(server, {worldComponent.tileSize, worldComponent.tileSize}, x * worldComponent.tileSize, y * worldComponent.tileSize);
    }
    gameEngine.registry.add_component(world, std::move(worldComponent));

    return world;
}

core::ecs::Entity EntityFactory::createPlayer(
    Server &server,
    const uint8_t id)
{
    auto &gameEngine = server.getGameEngine();
    auto &networkingService = server.getNetworkingService();
    const auto &config = server.getConfigManager();
    const auto &playersConnection = server.getPlayersConnection();
    auto &players = server.getPlayers();

    const core::ecs::Entity world = gameEngine.registry.get_entities<World>()[0];
    const auto &worldComponent = gameEngine.registry.get_component<World>(world);

    const std::function onCollision = [&, id](const core::ecs::Entity& entity, const core::ecs::Entity& otherEntity) {
        *gameEngine.out << "Player " << static_cast<int>(id) << " collided" << std::endl;

        RequestType requestType = PlayerHit;
        {
            const auto &playerComponent = gameEngine.registry.get_component<Player>(entity);
            playerComponent->health -= 1;
            if (playerComponent->health == 0)
                requestType = PlayerDie;
        }

        server.sendRequestToPlayers(requestType, {id});

        if (requestType == PlayerDie) {
            *gameEngine.out << "Player " << static_cast<int>(id) << " died" << std::endl;

            gameEngine.run_collision(PLAYER, otherEntity);

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

    const auto size = sf::Vector2f(
        config.getValue<float>("/player/size/x", 99.0f),
        config.getValue<float>("/player/size/y", 51.0f)
    );

    const core::ecs::Entity player = gameEngine.registry.spawn_entity();
    const auto [fst, snd] = worldComponent->spawnPoints[rand() % worldComponent->spawnPoints.size()];
    const core::ge::TransformComponent transformComponent = {sf::Vector2f(static_cast<float>(fst), static_cast<float>(snd)), size, sf::Vector2f(1, 1), 0};
    const Player playerComponent = {id, 3, std::time(nullptr)};

    gameEngine.registry.add_component(player, Network{networkingService});
    gameEngine.registry.add_component(player, std::move(transformComponent));
    gameEngine.registry.add_component(player, core::ge::CollisionComponent{PLAYER, std::vector{sf::FloatRect(0, 0, size.x, size.y)},{
        {ENEMY, onCollision},
        {TILE, onCollision},
        {WORLD, [&, id](const core::ecs::Entity &entity, const core::ecs::Entity &otherEntity) {

            const auto &playerTransformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(entity);

            if (const auto &worldTransformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(otherEntity);
                playerTransformComponent->position.x < worldTransformComponent->position.x)
                playerTransformComponent->position.x = worldTransformComponent->position.x;
            else if (playerTransformComponent->position.x + playerTransformComponent->size.x > worldTransformComponent->position.x + worldTransformComponent->size.x)
                playerTransformComponent->position.x = worldTransformComponent->position.x + worldTransformComponent->size.x - playerTransformComponent->size.x;

            {
                const auto x = static_cast<uint32_t>(playerTransformComponent->position.x);
                const auto y = static_cast<uint32_t>(playerTransformComponent->position.y);

                server.sendRequestToPlayers(
                    PlayerMove,
                    {
                        static_cast<uint8_t>(id),
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
        }}}});
    gameEngine.registry.add_component(player, std::move(playerComponent));

    networkingService.sendRequest(
        *playersConnection[id].value(),
        GameStart,
        {});

    const auto &worldTransformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(world);
    const auto &scroll = static_cast<uint32_t>(worldTransformComponent->position.x);
    {
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
        auto x = static_cast<uint32_t>(fst);
        auto y = static_cast<uint32_t>(snd);

        if (scroll > x) {
            for (const auto &otherPlayer : gameEngine.registry.get_entities<Player>()) {
                if (otherPlayer == player)
                    continue;
                const auto &otherPlayerTransformComponent = gameEngine.registry.get_component<core::ge::TransformComponent>(otherPlayer);
                x = static_cast<uint32_t>(otherPlayerTransformComponent->position.x);
                y = static_cast<uint32_t>(otherPlayerTransformComponent->position.y);
                break;
            }
        }

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

    *gameEngine.out << "Player " << static_cast<int>(id) << " created" << std::endl;
    return player;
}

core::ecs::Entity EntityFactory::createEnemy(Server &server, const uint32_t x, uint8_t enemyType)
{
    static uint8_t id = 0;
    if (id >= 255)
        return core::ecs::Entity{};

    auto &gameEngine = server.getGameEngine();
    auto &networkingService = server.getNetworkingService();
    const auto &config = server.getConfigManager();
    const auto &playersConnection = server.getPlayersConnection();

    const auto currentId = id;
    const std::function onCollision = [&, currentId](const core::ecs::Entity& entity, const core::ecs::Entity& otherEntity) {
        *gameEngine.out << "Enemy " << static_cast<int>(currentId) << " collided" << std::endl;

        gameEngine.run_collision(ENEMY, otherEntity);

        server.sendRequestToPlayers(EnemyDie, {currentId});
        gameEngine.registry.kill_entity(entity);
    };

    const core::ecs::Entity enemy = gameEngine.registry.spawn_entity();

    const std::string enemyConfigPath = "/enemies/" + std::to_string(enemyType);
    const auto size = sf::Vector2f(
        config.getValue<float>(enemyConfigPath + "/size/x", 115.0f),
        config.getValue<float>(enemyConfigPath + "/size/y", 126.0f)
    );

    const auto height = config.getValue<int>("/view/size/y", 1080.0f);
    const sf::Vector2i position = {static_cast<int>(x), rand() % (height - (height / 6) + height / 3)};
    gameEngine.registry.add_component(enemy, Network{networkingService});
    gameEngine.registry.add_component(enemy, core::ge::VelocityComponent{
        -config.getValue<float>(enemyConfigPath + "/speed/x", 115.0f),
        config.getValue<float>(enemyConfigPath + "/speed/y", 126.0f)});
    gameEngine.registry.add_component(enemy, core::ge::TransformComponent{sf::Vector2f(position), size, sf::Vector2f(1, 1), 0});
    gameEngine.registry.add_component(enemy, core::ge::CollisionComponent{ENEMY, std::vector{sf::FloatRect(0, 0, size.x, size.y)},{
        {PLAYER, onCollision},
        {PLAYER_PROJECTILE, onCollision},
        {TILE, onCollision},
        {WORLD, onCollision}}});
    gameEngine.registry.add_component(enemy, Enemy{id});

    const std::vector payload = {
        id,
        enemyType,
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

    *gameEngine.out << "Enemy " << static_cast<int>(id++) << " created" << std::endl;
    return enemy;
}

core::ecs::Entity EntityFactory::createProjectile(
    Server &server,
    const core::ecs::Entity &player)
{
    static uint8_t id = 0;
    if (id >= 255) {
        id = 0;
        return core::ecs::Entity{};
    }

    auto &gameEngine = server.getGameEngine();
    const auto &config = server.getConfigManager();

    const uint8_t currentId = id;
    const auto onCollision = [&, currentId](const core::ecs::Entity& entity, const core::ecs::Entity& otherEntity) {
        *gameEngine.out << "Projectile " << static_cast<int>(currentId) << " died" << std::endl;

        gameEngine.run_collision(PLAYER_PROJECTILE, otherEntity);
        gameEngine.registry.kill_entity(entity);

        server.sendRequestToPlayers(PlayerProjectileDestroy, {currentId});
    };

    const core::ecs::Entity projectile = gameEngine.registry.spawn_entity();

    const auto &playerTransform = gameEngine.registry.get_component<core::ge::TransformComponent>(player);

    const auto pos = sf::Vector2f(
        playerTransform->position.x + playerTransform->size.x,
        playerTransform->position.y + playerTransform->size.y / 2
    );
    const auto size = sf::Vector2f(
        config.getValue<float>("/player/weapons/0/size/x", 72.0f),
        config.getValue<float>("/player/weapons/0/size/y", 20.0f)
    );

    gameEngine.registry.add_component(projectile, core::ge::VelocityComponent{config.getValue<float>("/player/weapons/0/speed/x", 500.0f), config.getValue<float>("/player/weapons/0/speed/y", 0.0f)});
    gameEngine.registry.add_component(projectile, core::ge::TransformComponent{pos, size, sf::Vector2f(1, 1), 0});
    gameEngine.registry.add_component(projectile, core::ge::CollisionComponent{PLAYER_PROJECTILE, std::vector{sf::FloatRect(0, 0, size.x, size.y)},{
        {ENEMY, onCollision},
        {WORLD, onCollision},
        {TILE, onCollision}}});
    gameEngine.registry.add_component(projectile, Projectile{id});


    {
        const auto x = static_cast<uint32_t>(pos.x);
        const auto y = static_cast<uint32_t>(pos.y);

        server.sendRequestToPlayers(PlayerProjectileCreate, {
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

    *gameEngine.out << "Projectile " << static_cast<int>(id) << " created" << std::endl;
    return projectile;
}

core::ecs::Entity EntityFactory::createMissile(
    Server &server,
    const core::ecs::Entity &player)
{
    static uint8_t id = 0;
    if (id >= 255) {
        id = 0;
        return core::ecs::Entity{};
    }

    auto &gameEngine = server.getGameEngine();
    const auto &config = server.getConfigManager();

    const uint8_t currentId = id;
    const auto onCollision = [&, currentId](const core::ecs::Entity& entity, const core::ecs::Entity& otherEntity) {
        *gameEngine.out << "Projectile " << static_cast<int>(currentId) << " died" << std::endl;

        gameEngine.run_collision(PLAYER_PROJECTILE, otherEntity);
        gameEngine.registry.kill_entity(entity);

        server.sendRequestToPlayers(PlayerMissileDestroy, {currentId});
    };

    const core::ecs::Entity projectile = gameEngine.registry.spawn_entity();

    const auto &playerTransform = gameEngine.registry.get_component<core::ge::TransformComponent>(player);

    const auto size = sf::Vector2f(
        config.getValue<float>("/player/weapons/1/size/x", 136.0f),
        config.getValue<float>("/player/weapons/1/size/y", 48.0f)
    );

    const auto pos = sf::Vector2f(
        playerTransform->position.x + playerTransform->size.x,
        playerTransform->position.y + playerTransform->size.y / 2
    );
    gameEngine.registry.add_component(projectile, core::ge::VelocityComponent{config.getValue<float>("/player/weapons/1/speed/x", 500.0f), config.getValue<float>("/player/weapons/1/speed/y", 0.0f)});
    gameEngine.registry.add_component(projectile, core::ge::TransformComponent{pos, size, sf::Vector2f(1, 1), 0});
    gameEngine.registry.add_component(projectile, core::ge::CollisionComponent{PLAYER_PROJECTILE, std::vector{sf::FloatRect(0, 0, size.x, size.y)},{
        {ENEMY, onCollision},
        {WORLD, onCollision},
        {TILE, onCollision}}});
    gameEngine.registry.add_component(projectile, Projectile{id});

    {
        const auto x = static_cast<uint32_t>(pos.x);
        const auto y = static_cast<uint32_t>(pos.y);

        server.sendRequestToPlayers(PlayerMissileCreate, {
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

    *gameEngine.out << "Missile " << static_cast<int>(id) << " created" << std::endl;
    return projectile;
}


core::ecs::Entity EntityFactory::createTile(
    Server &server,
    const std::pair<uint32_t, uint32_t> &size,
    const uint32_t x,
    const uint32_t y)
{
    core::GameEngine &gameEngine = server.getGameEngine();

    const auto onCollision = [&](const core::ecs::Entity& entity, const core::ecs::Entity& otherEntity) {
        *gameEngine.out << "Tile collided" << std::endl;

        gameEngine.run_collision(TILE, otherEntity);
        server.sendRequestToPlayers(TileDestroy, {
            static_cast<uint8_t>(x >> 24),
            static_cast<uint8_t>(x >> 16),
            static_cast<uint8_t>(x >> 8),
            static_cast<uint8_t>(x),
            static_cast<uint8_t>(y >> 24),
            static_cast<uint8_t>(y >> 16),
            static_cast<uint8_t>(y >> 8),
            static_cast<uint8_t>(y)
        });
        gameEngine.registry.kill_entity(entity);
    };

    const core::ecs::Entity tile = gameEngine.registry.spawn_entity();

    gameEngine.registry.add_component(tile, core::ge::TransformComponent{sf::Vector2f(static_cast<float>(x * size.first), static_cast<float>(y * size.second)), sf::Vector2f(static_cast<float>(size.first), static_cast<float>(size.second)), sf::Vector2f(1, 1), 0});
    gameEngine.registry.add_component(tile, core::ge::CollisionComponent{TILE, std::vector{sf::FloatRect(0, 0, static_cast<float>(size.first), static_cast<float>(size.second))}, {
        {PLAYER_PROJECTILE, onCollision},
        {ENEMY, onCollision},
        {PLAYER, onCollision}}});

    return tile;
}
