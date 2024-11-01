---
title: Physics Engine
---

# Physics Engine

The R-Type game engine includes a basic physics system that simulates realistic movement, collisions, and forces. This document explains how to use the physics components and system.

## Components

### PhysicsComponent

The `PhysicsComponent` defines the physical properties of an entity:

```cpp
struct PhysicsComponent {
    float mass = 1.0f; // Mass in kg
    float elasticity = 0.5f; // Bounce factor (0-1)
    float friction = 0.1f; // Friction coefficient
    bool isStatic = false; // Static objects don't move
    sf::Vector2f acceleration{0.0f, 0.0f}; // Current acceleration
    sf::Vector2f forces{0.0f, 0.0f}; // Sum of forces
};
```

Properties:
- `mass`: The mass of the entity in kilograms. Affects how forces impact the entity.
- `elasticity`: Bounce factor between 0 (no bounce) and 1 (perfect bounce).
- `friction`: Friction coefficient that slows down movement.
- `isStatic`: If true, the object won't move but will affect other objects.
- `acceleration`: Current acceleration vector.
- `forces`: Sum of all forces currently acting on the entity.

### GravityComponent

The `GravityComponent` makes entities affected by gravity:
```cpp
struct GravityComponent {
    sf::Vector2f gravity{0.0f, 300.0f}; // Default gravity force
};
```

## Usage Example

Here's an example of creating a physics-enabled ball entity:

```cpp
core::ecs::Entity createBall(Game &game, const sf::Vector2f& position, const sf::Vector2f& velocity)
{
    auto& registry = game.getGameEngine().registry;
    core::ecs::Entity ball = registry.spawn_entity();
    // Add physics properties
    registry.add_component(ball, core::ge::PhysicsComponent{
    .mass = 500.0f,
    .elasticity = 0.5f,
    .friction = 0.0f,
    .isStatic = false,
    .acceleration = {10, 10},
    .forces = {10, 10}
    });
    // Add gravity
    registry.add_component(ball, core::ge::GravityComponent{});
    // Add velocity
    registry.add_component(ball, core::ge::VelocityComponent{velocity.x, velocity.y});
    return ball;
}
```

## Physics System

The physics system updates entity positions based on forces, gravity, and collisions. It processes entities with both `PhysicsComponent` and `VelocityComponent`.

Key features:
- Force accumulation
- Gravity application
- Velocity updates based on forces
- Friction application
- Collision response with elasticity

### How It Works

1. Forces are accumulated in the `PhysicsComponent`
2. Acceleration is calculated using F = ma
3. Velocity is updated based on acceleration
4. Friction is applied to slow down movement
5. Position is updated based on final velocity
6. Forces are cleared for the next frame

## Collision Response

The physics system includes basic collision response with elasticity. When objects collide:

1. Normal force is calculated
2. Velocity is reflected based on collision normal
3. Elasticity is applied to determine bounce strength
4. Objects are separated to prevent overlap

Example collision response:

```cpp
if (transform->position.y >= windowHeight - objectHeight) {
    // Ground collision
    transform->position.y = windowHeight - objectHeight;
    velocity->dy = -std::abs(velocity->dy) physics->elasticity;
    physics->forces.y = -std::abs(physics->forces.y) physics->elasticity;
}
```

## Best Practices

1. **Mass Values**: Use realistic mass ratios between objects
2. **Elasticity**: Keep between 0-1 for realistic behavior
3. **Static Objects**: Use for immovable objects like walls
4. **Force Application**: Clear forces each frame using `physics->forces = {0.0f, 0.0f}`
5. **Gravity**: Only add `GravityComponent` to objects that should fall

## Performance Considerations

- The physics system processes all entities with physics components each frame
- Static objects skip physics calculations
- Consider using spatial partitioning for many physics objects
- Adjust physics timestep (`delta_t`) for stability
