#include "Systems.hpp"

#include "Components.hpp"
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"
#include "../../../game/RequestType.hpp"


void Systems::worldSystem(core::ecs::Registry &registry)
{
    registry.add_system<Network, World>(
        [&]([[maybe_unused]] const core::ecs::Entity &entity, [[maybe_unused]] Network &network, World &world) {
            world.scroll += world.speed;

            const std::vector payload = {
                static_cast<uint8_t>(world.scroll >> 24),
                static_cast<uint8_t>(world.scroll >> 16),
                static_cast<uint8_t>(world.scroll >> 8),
                static_cast<uint8_t>(world.scroll)
            };

            for (const auto &playerEntity : registry.get_entities<Player>()) {
                const auto &playerComponent = registry.get_component<Player>(playerEntity);

                network.service.sendRequest(
                    playerComponent->endpoint,
                    MapScroll,
                    payload);
            }
        });
}

void Systems::playerSystem(core::ecs::Registry &registry, std::array<std::optional<core::ecs::Entity>, 4> &players)
{
    registry.add_system<Network, Player>(
        [&](const core::ecs::Entity &entity, const Network &network, Player &player) {
            if (player.lastTimePacketReceived + 5 < std::time(nullptr)) {
                for (auto &playerEntity : players) {
                    if (!playerEntity.has_value())
                        continue;

                    const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                    network.service.sendRequest(
                        playerComponent->endpoint,
                        PlayerDisconnect,
                        {player.id});
                }

                registry.kill_entity(entity);
                players[player.id].reset();
            }
        });
}

void Systems::enemySystem(core::ecs::Registry &registry, const std::array<std::optional<core::ecs::Entity>, 4> &players)
{
    registry.add_system<Network, Enemy>(
        [&](const core::ecs::Entity &entity, const Network &network, const Enemy &enemy) {
            const auto &transformComponent = registry.get_component<core::ge::TransformComponent>(entity);
            transformComponent->position.x -= 10;

            const sf::Vector2i position = {static_cast<int>(transformComponent->position.x), static_cast<int>(transformComponent->position.y)};
            const std::vector payload = {
                enemy.id,
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
                network.service.sendRequest(
                    playerComponent->endpoint,
                    EnemyMove,
                    payload);
            }
        });
}

void Systems::projectileSystem(core::ecs::Registry &registry)
{
    registry.add_system<Projectile>(
        [&](const core::ecs::Entity &entity, const Projectile &) {
            const auto &transformComponent = registry.get_component<core::ge::TransformComponent>(entity);
            transformComponent->position.x += 10;
        });
}
