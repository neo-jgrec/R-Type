# Using the Entity Component System (ECS)

The Entity Component System (ECS) is a core architectural pattern used in this project. It provides a flexible and efficient way to structure game objects and their behaviors.

## Creating Entities

Entities are created using the `Registry` class. Here's how you can create a new entity:

```cpp
core::ecs::Entity entity = registry.spawn_entity();
```

## Adding Components

Components are added to entities using the `add_component` method of the `Registry` class. For example, to add a `TransformComponent` to an entity:

```cpp
registry.add_component(entity, core::ge::TransformComponent{position, size, scale, rotation});
```

You can see an example of this in the `EntityFactory::createPlayer` method:


```cpp
core::ecs::Entity EntityFactory::createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position, int color, Game &game, sf::Vector2f gameScale)
{
    core::ecs::Entity player = registry.spawn_entity();

    registry.add_component(player, core::ge::TransformComponent{position, sf::Vector2f(33.0f, 17.0f), gameScale, 0.0f});
    registry.add_component(player, VelocityComponent{10.0f, 10.0f});

    ...
    return player;
}
```


## Creating Systems

Systems are created by adding them to the registry using the `add_system` method. Systems operate on entities with specific components. For example, the input system:


```cpp
void Game::inputSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, InputStateComponent, ShootCounterComponent>
    ([&](core::ecs::Entity, core::ge::TransformComponent &transform, const VelocityComponent &vel, InputStateComponent &input, ShootCounterComponent &shootCounter) {
            if (input.up)
                transform.position.y -= vel.dy;
            if (input.down)
                transform.position.y += vel.dy;
            if (input.left)
                transform.position.x -= vel.dx;
            if (input.right)
                transform.position.x += vel.dx;
        });
}
```


This system operates on entities that have `TransformComponent`, `VelocityComponent`, `InputStateComponent`, and `ShootCounterComponent`.

# Using the Game Engine

The Game Engine provides a high-level interface for managing game objects, rendering, and game logic.

## Initializing the Game Engine

The Game Engine is initialized in the `Game` class constructor:

## Running the Game Loop

The game loop is implemented in the `run` method of the `Game` class:


```cpp
void Game::run() {
    while (_windowOpen) {
        sf::Time elapsed = _gameEngine.clock.restart();
        _gameEngine.delta_t = elapsed.asSeconds();

        processEvents();
        update();
        render();
    }
}
```


This loop continuously processes events, updates the game state, and renders the scene.

## Adding Game Objects

Game objects (entities with components) are typically added using factory methods. For example, the `EntityFactory::createPlayer` method:


```cpp
core::ecs::Entity EntityFactory::createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position, int color, Game &game, sf::Vector2f gameScale)
{
    core::ecs::Entity player = registry.spawn_entity();

    registry.add_component(player, core::ge::TransformComponent{position, sf::Vector2f(33.0f, 17.0f), gameScale, 0.0f});
    registry.add_component(player, core::ge::CollisionComponent{PLAYER, {sf::FloatRect(0.0f, 0.0f, 33.0f, 17.0f)}, {
        { ENEMY, [&](const core::ecs::Entity self, [[maybe_unused]] const core::ecs::Entity other) {
                registry.kill_entity(self);
                game.releaseColor(color);
        }}}});
    registry.add_component(player, VelocityComponent{10.0f, 10.0f});
    registry.add_component(player, InputStateComponent{});
    registry.add_component(player, core::ge::KeyBinding{});
    registry.add_component(player, HealthComponent{10});
    registry.add_component(player, ScoreComponent{0});
    registry.add_component(player, Player{});
    registry.add_component(player, ShootCounterComponent{0});
    registry.add_component(player, PlayerColorComponent{color});

    std::string relativePath = "assets/player_sprite.png";
    std::string absolutePath = std::filesystem::absolute(relativePath).string();
    auto texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(absolutePath)) {
        std::cerr << "Failed to load texture: " << absolutePath << std::endl;
        return player;
    }

    sf::RectangleShape playerShape(sf::Vector2f(33.0f, 17.0f));
    playerShape.setTexture(texture.get());

    int row = color * 17;
    playerShape.setTextureRect(sf::IntRect(0, row, 33, 17));

    registry.add_component(player, core::ge::DrawableComponent{playerShape});
    registry.add_component(player, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});
    registry.add_component(player, core::ge::TextureComponent{texture});

    std::vector<sf::IntRect> moveFrames;
    moveFrames.reserve(5);
    for (int i = 0; i < 5; i++) {
        moveFrames.emplace_back(i * 33, row, 33, 17);
    }
    registry.add_component(player, core::ge::AnimationComponent{
        .animations = {
            {core::ge::AnimationState::Moving, moveFrames}
        },
        .frameTime = 0.1f,
        .elapsedTime = 0.0f,
        .currentFrame = 0,
        .loop = true
    });

    return player;
}

```


# Algorithmic Choices and Justifications

## Sparse Array for Component Storage

We chose to use a Sparse Array data structure for storing components. This decision was made for several reasons:

1. **Memory Efficiency**: Sparse arrays allow us to efficiently store components for entities with non-contiguous IDs.
2. **Fast Access**: Accessing components by entity ID is an O(1) operation.
3. **Flexibility**: It's easy to add or remove components dynamically.


## Event Pool for Event Management

We implemented an Event Pool system for managing game events. This choice was made for the following reasons:

1. **Decoupling**: It allows us to decouple event generation from event handling.
2. **Efficient Processing**: Events can be processed in batches, which is more efficient than processing them immediately as they occur.
3. **Thread Safety**: The event pool can be made thread-safe, allowing for concurrent event generation and processing.

### Getting Started

The `EventPool` is designed to be used as a "singleton." This means that there is only one `EventPool` in the game, and everyone uses the same one. To get this instance, you use the `getInstance` function:

```cpp
// Get access to the EventPool
EventPool& eventPool = EventPool::getInstance();
```

Now, you have access to the `EventPool` and can start adding or retrieving events.

### Adding an Event

To add a new event to the `EventPool`, you use the `addEvent` function. Let’s say a player has moved in the game:

```cpp
// Create a PlayerMovement event
PlayerMovement movement = {1, 100.0f, 200.0f, 0.0f}; // Player ID 1 moves to (100, 200, 0)
Event movementEvent(0x01, movement); // 0x01 is the event type for PlayerMovement

// Add this event to the pool
eventPool.addEvent(movementEvent);
```

This will put the movement event in the queue, ready to be processed later.

### Getting the Next Event

Once you have added events to the `EventPool`, you might want to retrieve the next one to see what needs to be handled:

```cpp
// Get the next event in the queue
Event nextEvent = eventPool.getNextEvent();
```

This will give you the next event from the queue and remove it, so you can handle it in your game.

### Getting All Events at Once

If you want to process all the events that have been stored in the `EventPool`, you can get all of them at once:

```cpp
// Retrieve and process all events
...
            // Handle player movement
        } else if (e.getType() == 0x05) {
            // Handle player shoot
        }
        // Add more cases for other types of events...
    }
}
```

## Summary

The `EventPool` makes it easy to keep track of all the actions happening in the game. You can add new events when things happen (like a player moving or an enemy appearing), and then process them later when you’re ready. It’s like a to-do list for game events!

## Custom Network Protocol (GDTP)

We developed a custom network protocol, Game Datagram Transfer Protocol (GDTP), for efficient game state synchronization. The reasons for this choice include:

1. **Low Latency**: By using UDP and a lightweight protocol, we minimize network latency.
2. **Bandwidth Efficiency**: The binary protocol reduces the amount of data that needs to be transferred.
3. **Flexibility**: The custom protocol allows us to easily add new message types as needed.

:::info
You can refer to the [protocol documentation](../../../Network/Game Datagram Transfer Protocol (GDTP)).
:::
