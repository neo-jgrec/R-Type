#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include "../../../core/ecs/Registry/Registry.hpp"
#include "../../../core/network/NetworkService.hpp"

class EntityFactory {
public:
    static core::ecs::Entity createConnectionHub(core::ecs::Registry& registry, const NetworkingService &networkingService, std::vector<core::ecs::Entity> &players);
    static core::ecs::Entity createWorld(core::ecs::Registry& registry, const NetworkingService &networkingService, const std::string& filePath);
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry, const NetworkingService &networkingService);
    static core::ecs::Entity createEnemy(core::ecs::Registry& registry, const NetworkingService &networkingService);
};

#endif //ENTITYFACTORY_HPP
