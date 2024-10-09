#include "Systems.hpp"
#include "Components.hpp"
#include "../../../game/MessageType.hpp"

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
            if (player.lastTimePacketReceived > 5000)
                registry.kill_entity(entity);
            players[player.id].reset();

            for (auto &playerEntity : players) {
                if (!playerEntity.has_value())
                    continue;

                const auto &playerComponent = registry.get_component<Player>(playerEntity.value());
                network.service.sendRequest(
                    playerComponent->endpoint,
                    PlayerDisconnect,
                    {player.id});
            }
        });
}
