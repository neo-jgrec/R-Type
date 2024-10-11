#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include "../../../core/ecs/Registry/Registry.hpp"
#include "../../../core/network/NetworkService.hpp"

class EntityFactory {
public:
    static core::ecs::Entity createWorld(core::ecs::Registry& registry, NetworkingService &networkingService, const std::string& filePath);
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry, NetworkingService &networkingService, const std::array<std::optional<core::ecs::Entity>, 4> &players, const asio::ip::udp::endpoint &endpoint, uint8_t id);
    static core::ecs::Entity createEnemy(core::ecs::Registry& registry, NetworkingService &networkingService, const std::array<std::optional<core::ecs::Entity>, 4> &players);
    static core::ecs::Entity createProjectile(core::ecs::Registry& registry, const core::ecs::Entity &player, uint8_t id);
};

#endif //ENTITYFACTORY_HPP
