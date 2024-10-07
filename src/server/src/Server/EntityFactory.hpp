#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include "../../../core/ecs/Registry/Registry.hpp"

class EntityFactory {
public:
    static core::ecs::Entity createWorld(core::ecs::Registry& registry);
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry);
    static core::ecs::Entity createEnemy(core::ecs::Registry& registry);
};

#endif //ENTITYFACTORY_HPP
