#include "EntityFactory.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "Components.hpp"
#include "../../../game/CollisionMask.hpp"
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"

core::ecs::Entity EntityFactory::createWorld(core::ecs::Registry& registry, const std::string& filePath)
{
    const core::ecs::Entity world = registry.spawn_entity();

    registry.add_component(world, Network{});
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

core::ecs::Entity EntityFactory::createPlayer(core::ecs::Registry& registry)
{
    const core::ecs::Entity player = registry.spawn_entity();

    uint8_t id = 0;
    for (const auto otherPlayers = registry.get_entities<Player>();
        const auto& otherPlayer : otherPlayers) {
        if (const auto ptr = std::any_cast<std::shared_ptr<Player>>(otherPlayer);
            ptr->id == id)
            id++;
    }
    if (id == 4)
        throw std::runtime_error("Maximum number of players reached");

    registry.add_component(player, Network{});
    registry.add_component(player, core::ge::TransformComponent{sf::Vector2f(0, 0), sf::Vector2f(32, 32), sf::Vector2f(1, 1), 0});

    registry.add_component(player, core::ge::CollisionComponent{PLAYER, std::vector{sf::FloatRect(0, 0, 32, 32)},{
        { ENEMY, [id]([[maybe_unused]] const core::ecs::Entity& entity, [[maybe_unused]] const core::ecs::Entity& otherEntity) {
            std::cout << "Player " << static_cast<int>(id) << " collided with enemy" << std::endl;
        }}}});
    registry.add_component(player, Player{id});

    std::cout << "Player " << static_cast<int>(id) << " created" << std::endl;
    return player;
}

core::ecs::Entity EntityFactory::createEnemy(core::ecs::Registry& registry)
{
    const core::ecs::Entity enemy = registry.spawn_entity();

    registry.add_component(enemy, Network{});
    registry.add_component(enemy, core::ge::TransformComponent{sf::Vector2f(0, 0), sf::Vector2f(32, 32), sf::Vector2f(1, 1), 0});
    registry.add_component(enemy, core::ge::CollisionComponent{ENEMY, std::vector{sf::FloatRect(0, 0, 32, 32)},{
        {PLAYER, []([[maybe_unused]] const core::ecs::Entity& entity, [[maybe_unused]] const core::ecs::Entity& otherEntity) {
            std::cout << "Enemy collided with player" << std::endl;
        }}}});
    registry.add_component(enemy, Enemy{});

    std::cout << "Enemy created" << std::endl;
    return enemy;
}
