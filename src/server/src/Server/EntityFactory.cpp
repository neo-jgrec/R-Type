#include "EntityFactory.hpp"
#include "Components.hpp"

core::ecs::Entity EntityFactory::createWorld(core::ecs::Registry& registry)
{
    const core::ecs::Entity world = registry.spawn_entity();

    registry.add_component(world, Network{});
    registry.add_component(world, Position{0, 0});
    registry.add_component(world, RectangleCollider{800, 600});
    registry.add_component(world, World{1, 0});

    return world;
}
