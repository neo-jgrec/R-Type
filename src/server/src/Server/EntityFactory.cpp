#include "EntityFactory.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>

#include "Components.hpp"
#include "../../../game/CollisionMask.hpp"
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"
#include "../../../game/RequestType.hpp"

core::ecs::Entity EntityFactory::createConnectionHub(
    core::ecs::Registry &registry,
    NetworkingService &networkingService,
    std::array<std::optional<core::ecs::Entity>, 4> &players)
{
    const core::ecs::Entity connectionHub = registry.spawn_entity();

    registry.add_component(connectionHub, Network{networkingService});
    registry.add_component(connectionHub, ConnectionHub{players});

    return connectionHub;
}

core::ecs::Entity EntityFactory::createWorld(
    core::ecs::Registry& registry,
    NetworkingService &networkingService,
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
        { json["width"], json["height"] },
        std::vector(json["width"], std::vector<uint8_t>(json["height"], 0)) };
    for (const auto& tile : json["tiles"]) {
        if (tile["x"] < 0 || tile["x"] >= worldComponent.size.first
            || tile["y"] < 0 || tile["y"] >= worldComponent.size.second) {
            throw std::out_of_range("Tile coordinates out of bounds");
        }
        worldComponent.tiles[tile["x"]][tile["y"]] = static_cast<uint8_t>(tile["tileIndex"]);
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

    registry.add_component(player, Network{networkingService});
    registry.add_component(player, core::ge::TransformComponent{sf::Vector2f(0, 0), sf::Vector2f(32, 32), sf::Vector2f(1, 1), 0});
    registry.add_component(player, core::ge::CollisionComponent{PLAYER, std::vector{sf::FloatRect(0, 0, 32, 32)},{
        { ENEMY, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Player " << id << " collided with enemy" << std::endl;

            {
                const auto &playerComponent = registry.get_component<Player>(entity);
                playerComponent->health -= 1;
                if (playerComponent->health > 0)
                    return;
            }

            for (auto &playerEntity : players) {
                if (!playerEntity.has_value())
                    continue;
                const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                networkingService.sendRequest(
                    playerComponent->endpoint,
                    PlayerDie,
                    {playerComponent->id});
            }

            registry.kill_entity(entity);
        }}}});
    registry.add_component(player, core::ge::CollisionComponent{PLAYER, std::vector{sf::FloatRect(0, 0, 32, 32)},{
        { TILE, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Player " << id << " collided with enemy" << std::endl;

            {
                const auto &playerComponent = registry.get_component<Player>(entity);
                playerComponent->health -= 1;
                if (playerComponent->health > 0)
                    return;
            }

            for (auto &playerEntity : players) {
                if (!playerEntity.has_value())
                    continue;
                const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                networkingService.sendRequest(
                    playerComponent->endpoint,
                    PlayerDie,
                    {playerComponent->id});
            }

            registry.kill_entity(entity);
        }}}});
    registry.add_component(player, Player{endpoint, id, 3, std::time(nullptr)});

    for (auto &playerEntity : players) {
        if (!playerEntity.has_value())
            continue;
        const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
        networkingService.sendRequest(
            playerComponent->endpoint,
            PlayerConnect,
            {id});
        if (playerEntity.value() == player)
            continue;
        // Send the existing players to the new player
        networkingService.sendRequest(
            endpoint,
            PlayerConnect,
            {playerComponent->id});
    }

    std::cout << "Player " << id << " created" << std::endl;
    return player;
}

core::ecs::Entity EntityFactory::createEnemy(
    core::ecs::Registry& registry,
    NetworkingService &networkingService,
    const std::array<std::optional<core::ecs::Entity>, 4> &players)
{
    static uint8_t id = 0;
    const core::ecs::Entity enemy = registry.spawn_entity();

    const sf::Vector2i position = {rand() % 800, rand() % 600};
    registry.add_component(enemy, Network{networkingService});
    registry.add_component(enemy, core::ge::TransformComponent{sf::Vector2f(position), sf::Vector2f(32, 32), sf::Vector2f(1, 1), 0});
    registry.add_component(enemy, core::ge::CollisionComponent{ENEMY, std::vector{sf::FloatRect(0, 0, 32, 32)},{
        { PLAYER, [&](const core::ecs::Entity& entity, const core::ecs::Entity&) {
            std::cout << "Enemy " << id << " collided with player" << std::endl;

            const auto &enemyComponent = registry.get_component<Enemy>(entity);
            for (auto &playerEntity : players) {
                if (!playerEntity.has_value())
                    continue;
                const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                networkingService.sendRequest(
                    playerComponent->endpoint,
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
            playerComponent->endpoint,
            EnemySpawn,
            payload);
    }

    std::cout << "Enemy " << id++ << " created" << std::endl;
    return enemy;
}
