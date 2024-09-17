#include <iostream>
#include "../../core/ecs/Registry/Registry.hpp"

struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

void print_positions_and_velocities(const Position &position, const Velocity &velocity)
{
    std::cout << "Entity has Position (" << position.x << ", " << position.y << ") "
              << "and is moving at (" << velocity.dx << ", " << velocity.dy << ")\n";
}

int main()
{
    core::ecs::Registry registry;

    registry.register_component<Position>();
    registry.register_component<Velocity>();

    auto entity1 = registry.spawn_entity();
    auto entity2 = registry.spawn_entity();

    registry.emplace_component<Position>(entity1, 1.0f, 2.0f);
    registry.emplace_component<Velocity>(entity1, 0.5f, 0.5f);

    registry.emplace_component<Position>(entity2, 3.0f, 4.0f);
    registry.emplace_component<Velocity>(entity2, 1.0f, 1.0f);

    registry.add_system<Position, Velocity>([](const Position &pos, const Velocity &vel)
                                            { print_positions_and_velocities(pos, vel); });

    registry.run_systems();

    return 0;
}
