#include "Systems.hpp"
#include "Components.hpp"
#include "PacketType.hpp"

void Systems::worldSystem(core::ecs::Registry &registry)
{
    registry.add_system<Network, World>(
        [&](const Network &network, World &world) {
            world.scroll += world.speed;

            const std::vector payload = {
                static_cast<uint8_t>(world.scroll >> 24),
                static_cast<uint8_t>(world.scroll >> 16),
                static_cast<uint8_t>(world.scroll >> 8),
                static_cast<uint8_t>(world.scroll)
            };

            for (const auto &playerEntity : registry.get_entities<Player>()) {
                const auto player = std::any_cast<std::shared_ptr<Player>>(playerEntity);

                network.service.sendRequest(
                    player->endpoint,
                    MapScrolling,
                    payload);
            }
        });
}
