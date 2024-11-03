# Game Engine Guide

This guide provides a quick start for developers using the Game Engine. Here, we’ll walk through setting up a game window, managing assets, adding entities and components, and running the game loop.

:::note
You can find the full source code for the Game Engine in the `pong` project of the R-Type repository.  
The pong game is a simple example that demonstrates the core features of the Game Engine.
:::

## Features
- **Entity-Component-System (ECS)** architecture for efficient entity management.
- **Customizable Components** like `VelocityComponent` and `PlayerControlComponent`.
- **Collision Detection** using bitmasks.
- **Asset Management** to load and manage textures and sounds.
- **Metrics System** for in-game statistics.

## Requirements
- SDL2 and SFML libraries (SFML is used for logic (vector, ...), it's mandatory and SDL2 for rendering in our case but we also support SFML for rendering)
- LuaBridge for Lua scripting support
- C++17 or later

## Setup

1. **Clone the repository** and ensure all dependencies (SDL2, SFML, LuaBridge) are installed.
2. **Build the project** using CMake or your preferred build system.

   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```

## Basic Structure

### Game Engine (`GameEngine.hpp`)

The `GameEngine` class initializes the game environment and manages the main game loop. This includes:
- **Entity registry** for adding/removing entities and components.
- **System management** to define and run systems (logic that applies to entities with specific components).
- **Asset loading** for textures and sounds.
- **Metrics** for tracking stats like scores and ball position.

### Main Game Loop (`main.cpp`)

The game loop in `main.cpp` initializes the game and runs it until the player exits. It also handles events, updates the state of entities, and renders the scene.

## Step-by-Step Usage

### 0. Include the Game Engine

Include the Game Engine header in your project:

```cpp
#include "GameEngine.hpp"
```

If you want to use the SDL instead of the default SFML for rendering, you can define the `GE_USE_SDL` macro before including the header:

```cpp
#define GE_USE_SDL
#include "GameEngine.hpp"
```

:::note
You can also define it at the CMake level by adding `-DGE_USE_SDL=1` to the `cmake` command.  
This will use SDL for rendering instead of SFML, and choosed at compile time.
:::

:::warning
We do not recommand passing the `-DGE_USE_SDL=1` flag to the `cmake` , because it change the whole development environment to use SDL instead of SFML.  
We recommand to define the `GE_USE_SDL` macro in the source code. So any developer can easily understand that the project is using SDL instead of SFML.
:::

### 1. Initialize the Game Engine

```cpp
core::GameEngine engine{false};  // 'false' so we'll not automatically initialize the window
engine.initWindow({800, 600}, 60, "Pong");
```

### 2. Load Assets

Assets like textures are loaded at the start. For example, loading textures for the player and ball:

```cpp
engine.assetManager.loadTexture("ball", "assets/logo.png");
engine.assetManager.loadTexture("player", "assets/pong/white.png");
```

### 3. Define Custom Components

In `main.cpp`, several components are defined to represent entity properties:
- **VelocityComponent**: Stores x and y velocity.
- **PlayerControlComponent**: Defines the player’s control keys.
- **PlayerScoreComponent**: Stores the player's score.

```cpp
struct VelocityComponent { int vx, vy; };
struct PlayerControlComponent { SDL_Scancode upKey, downKey; };
struct PlayerScoreComponent { short score; };
```

### 4. Register Components and Add Systems

Register custom components with the engine and define systems for game mechanics:

```cpp
engine.registry.register_component<VelocityComponent>();
engine.registry.register_component<PlayerControlComponent>();
engine.registry.register_component<PlayerScoreComponent>();

engine.registry.add_system<core::ge::TransformComponent, VelocityComponent, core::ge::DrawableComponent>(
    [](core::ecs::Entity, core::ge::TransformComponent &transform, VelocityComponent &velocity, core::ge::DrawableComponent &drawable) {
        transform.position.x += velocity.vx;
        transform.position.y += velocity.vy;
        drawable.shape.x = static_cast<int>(transform.position.x);
        drawable.shape.y = static_cast<int>(transform.position.y);
    });
```

### 5. Create Entities and Assign Components

Each entity in the game is assigned various components. For example, create a player entity with position, control, and score components:

```cpp
core::ecs::Entity player1 = engine.registry.spawn_entity();
engine.registry.add_component<core::ge::TransformComponent>(player1, { {10, 10}, {20, 100}, {1, 1}, 0 });
engine.registry.add_component<PlayerScoreComponent>(player1, {0});
engine.registry.add_component<VelocityComponent>(player1, {0, 0});
engine.registry.add_component<PlayerControlComponent>(player1, {SDL_SCANCODE_W, SDL_SCANCODE_S});
```

### 6. Run the Game Loop

The main loop handles events, updates systems, and renders frames. Metrics can be toggled by pressing the "M" key, and the game exits when `ESC` is pressed.

```cpp
bool isRunning = true;
while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            isRunning = false;
        }
    }
    // Run systems and update entities
    engine.registry.run_system<core::ge::TransformComponent, VelocityComponent, core::ge::DrawableComponent>();
    engine.registry.run_system<core::ge::TransformComponent, PlayerControlComponent, core::ge::DrawableComponent>();

    // Render the frame
    SDL_RenderClear(engine.renderer);
    engine.registry.run_system<core::ge::DrawableComponent>();
    SDL_RenderPresent(engine.renderer);
}
```

## Additional Features
Those are some additional features that the Game Engine provides that we didn't cover in the basic structure.

### Collision Detection
Entities with `CollisionComponent` can participate in collision detection. Define collision masks to handle specific entity interactions, such as between players and the ball.

```cpp
engine.registry.add_component<core::ge::CollisionComponent>(player1, {PLAYER, {sf::FloatRect(0.0f, 0.0f, 20, 100)}});
```

### Metrics Display
The engine supports adding metrics for debugging or display. Toggle metrics on/off using `M` key.

```cpp
engine.addMetrics("Player 1 Score", [&engine, player1]() {
    return std::to_string(engine.registry.get_component<PlayerScoreComponent>(player1)->score);
});
```

### Shell for commands
The engine provides a shell for running commands during gameplay. Define commands and bind them to actions (can be cheats, debug, moderation, etc.).
You can see how it works at : [Shell](Command%20Line%20Interface.md)

### Lua Scripting

The engine supports Lua scripting for defining game logic. Load Lua scripts to define custom behavior for entities, components, and systems.

```lua
-- Function that print a message taken as parameter
function printMessage(message)
    print(message)
    return message
end
```

And now to call this function from C++:

```cpp
engine.run_script("assets/scripts/example.lua", "printMessage", std::string("Script says : Metrics updated (every secs)!"));
```

It runs the `printMessage` function from the `example.lua` script with the message `Script says : Metrics updated (every secs)!`.
