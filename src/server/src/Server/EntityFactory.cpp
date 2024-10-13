#include "EntityFactory.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>

#include "Components.hpp"
#include "../../../game/CollisionMask.hpp"
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"
#include "../../../game/RequestType.hpp"

core::ecs::Entity EntityFactory::createWorld(
    core::ecs::Registry& registry,
    NetworkingService &networkingService,
    const std::array<std::optional<core::ecs::Entity>, 4> &players,
    const std::string& filePath)
{
    const core::ecs::Entity world = registry.spawn_entity();

    registry.add_component(world, Network{networkingService});
    registry.add_component(world, core::ge::TransformComponent{sf::Vector2f(0, 0), sf::Vector2f(800, 600), sf::Vector2f(1, 1), 0});
    registry.add_component(world, core::ge::CollisionComponent{WORLD, std::vector{sf::FloatRect(0, 0, 800, 600)}});

    nlohmann::json json;
    {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }
        file >> json;
    }

    World worldComponent = {1, 0,
        { json["width"], json["height"] }};
    for (const auto& tile : json["tiles"]) {
        if (tile["x"] < 0 || tile["x"] >= worldComponent.size.first
            || tile["y"] < 0 || tile["y"] >= worldComponent.size.second) {
            throw std::out_of_range("Tile coordinates out of bounds");
        }
        createTile(registry, networkingService, players, {32, 32}, tile["x"], tile["y"]);
    }
    registry.add_component(world, std::move(worldComponent));

    return world;
}

core::ecs::Entity EntityFactory::createPlayer(
    core::ecs::Registry& registry,
    NetworkingService &networkingService,
    const std::array<std::optional<core::ecs::Entity>, 4> &players,
    const asio::ip::udp::endpoint &endpoint,
    const uint8_t id)
{
    const core::ecs::Entity player = registry.spawn_entity();

    const std::function onCollision = [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
        std::cout << "Player " << static_cast<int>(id) << " collided with enemy" << std::endl;

        RequestType requestType = PlayerHit;
        {
            const auto &playerComponent = registry.get_component<Player>(entity);
            playerComponent->health -= 1;
            if (playerComponent->health > 0)
                requestType = PlayerDie;
        }

        for (auto &playerEntity : players) {
            if (!playerEntity.has_value())
                continue;
            const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
            networkingService.sendRequest(
                *playerComponent->endpoint,
                requestType,
                {playerComponent->id});
        }

        if (requestType == PlayerDie)
            registry.kill_entity(entity);

        if (std::ranges::any_of(players, [&](const auto &playerEntity) {
            if (!playerEntity.has_value())
                return false;
            if (const auto &playerComponent = registry.get_component<Player>(playerEntity.value()); playerComponent->health > 0)
                return true;
            return false;
        }))
            for (auto &playerEntity : players) {
                if (!playerEntity.has_value())
                    continue;
                const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                networkingService.sendRequest(
                    *playerComponent->endpoint,
                    GameOver,
                    {});
            }
    };

    registry.add_component(player, Network{networkingService});
    registry.add_component(player, core::ge::TransformComponent{sf::Vector2f(0, 0), sf::Vector2f(32, 32), sf::Vector2f(1, 1), 0});
    registry.add_component(player, core::ge::CollisionComponent{PLAYER, std::vector{sf::FloatRect(0, 0, 32, 32)},{
        { ENEMY, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Player " << static_cast<int>(id) << " collided with enemy" << std::endl;

            RequestType requestType = PlayerHit;
            {
                const auto &playerComponent = registry.get_component<Player>(entity);
                playerComponent->health -= 1;
                if (playerComponent->health > 0)
                    requestType = PlayerDie;
            }

            for (auto &playerEntity : players) {
                if (!playerEntity.has_value())
                    continue;
                const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                networkingService.sendRequest(
                    *playerComponent->endpoint,
                    requestType,
                    {playerComponent->id});
            }

            if (requestType == PlayerDie)
               registry.kill_entity(entity);
        }},
        { TILE, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Player " << static_cast<int>(id) << " collided with enemy" << std::endl;

            RequestType requestType = PlayerHit;
            {
                const auto &playerComponent = registry.get_component<Player>(entity);
                playerComponent->health -= 1;
                if (playerComponent->health > 0)
                    requestType = PlayerDie;
            }

            for (auto &playerEntity : players) {
                if (!playerEntity.has_value())
                    continue;
                const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                networkingService.sendRequest(
                    *playerComponent->endpoint,
                    requestType,
                    {playerComponent->id});
            }

            if (requestType == PlayerDie)
                registry.kill_entity(entity);
        }}}});

    registry.add_component(player, Player{std::make_shared<asio::ip::udp::endpoint>(endpoint), id, 3, 0});


    for (auto &playerEntity : players) {
        if (!playerEntity.has_value())
            continue;

        const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
        const auto &playerTransform = registry.get_component<core::ge::TransformComponent>(playerEntity.value());

        // if (playerComponent->id != id) {
        //     networkingService.sendRequest(
        //         *playerComponent->endpoint,
        //         PlayerConnect,
        //         {playerComponent->id});
        //     const auto x = static_cast<uint8_t>(playerTransform->position.x);
        //     const auto y = static_cast<uint8_t>(playerTransform->position.y);
        //     const std::vector payload = {
        //         id,
        //         static_cast<uint8_t>(x >> 24),
        //         static_cast<uint8_t>(x >> 16),
        //         static_cast<uint8_t>(x >> 8),
        //         static_cast<uint8_t>(x),
        //         static_cast<uint8_t>(y >> 24),
        //         static_cast<uint8_t>(y >> 16),
        //         static_cast<uint8_t>(y >> 8),
        //         static_cast<uint8_t>(y)
        //     };
        //
        //     networkingService.sendRequest(
        //         endpoint,
        //         PlayerMove,
        //         payload);
        // }
    }

    std::cout << "Player " << static_cast<int>(id) << " created" << std::endl;
    return player;
}

core::ecs::Entity EntityFactory::createEnemy(
    core::ecs::Registry& registry,
    NetworkingService &networkingService,
    const std::array<std::optional<core::ecs::Entity>, 4> &players,
    const uint32_t x)
{
    static uint8_t id = 0;
    if (id >= 255)
        return core::ecs::Entity{};

    const core::ecs::Entity enemy = registry.spawn_entity();

    const sf::Vector2i position = {static_cast<int>(x), rand() % 880 + 100};
    registry.add_component(enemy, Network{networkingService});
    registry.add_component(enemy, core::ge::TransformComponent{sf::Vector2f(position), sf::Vector2f(32, 32), sf::Vector2f(1, 1), 0});
    registry.add_component(enemy, core::ge::CollisionComponent{ENEMY, std::vector{sf::FloatRect(0, 0, 32, 32)},{
        { PLAYER, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Enemy " << static_cast<int>(id) << " collided with player" << std::endl;

            const auto &enemyComponent = registry.get_component<Enemy>(entity);
            for (auto &playerEntity : players) {
                if (!playerEntity.has_value())
                    continue;
                const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                networkingService.sendRequest(
                    *playerComponent->endpoint,
                    EnemyDie,
                    {enemyComponent->id});
            }

            registry.kill_entity(entity);
        }}}});
    registry.add_component(enemy, Enemy{id});

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
    for (auto &playerEntity : players) {
        if (!playerEntity.has_value())
            continue;
        const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
        networkingService.sendRequest(
            *playerComponent->endpoint,
            EnemySpawn,
            payload);
    }

    std::cout << "Enemy " << static_cast<int>(id++) << " created" << std::endl;
    return enemy;
}

core::ecs::Entity EntityFactory::createProjectile(
    core::ecs::Registry &registry,
    const core::ecs::Entity &player,
    const uint8_t id)
{
    const core::ecs::Entity projectile = registry.spawn_entity();

    const auto &playerTransform = registry.get_component<core::ge::TransformComponent>(player);

    registry.add_component(projectile, core::ge::TransformComponent{playerTransform->position, sf::Vector2f(8, 8), sf::Vector2f(1, 1), 0});
    registry.add_component(projectile, core::ge::CollisionComponent{PLAYER_PROJECTILE, std::vector{sf::FloatRect(0, 0, 8, 8)},{
        { ENEMY, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            registry.kill_entity(entity);
        }},
        { WORLD, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            registry.kill_entity(entity);
        }},
        { TILE, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            registry.kill_entity(entity);
        }}}});
    registry.add_component(projectile, Projectile{id});

    std::cout << "Projectile created" << std::endl;
    return projectile;
}

core::ecs::Entity EntityFactory::createTile(
    core::ecs::Registry& registry,
    NetworkingService &networkingService,
    const std::array<std::optional<core::ecs::Entity>, 4> &players,
    const std::pair<uint32_t, uint32_t> &size,
    const uint32_t x,
    const uint32_t y)
{
    const core::ecs::Entity tile = registry.spawn_entity();

    registry.add_component(tile, core::ge::TransformComponent{sf::Vector2f(static_cast<float>(x * size.first), static_cast<float>(y * size.second)), sf::Vector2f(static_cast<float>(size.first), static_cast<float>(size.second)), sf::Vector2f(1, 1), 0});
    registry.add_component(tile, core::ge::CollisionComponent{TILE, std::vector{sf::FloatRect(0, 0, static_cast<float>(size.first), static_cast<float>(size.second))}, {
        { PLAYER_PROJECTILE, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Tile collided with projectile" << std::endl;

            for (auto &playerEntity : players) {
                if (!playerEntity.has_value())
                    continue;
                const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                networkingService.sendRequest(
                    *playerComponent->endpoint,
                    TileDestroy,
                    {playerComponent->id});
            }

            registry.kill_entity(entity);
        }}}});

    return tile;
}
