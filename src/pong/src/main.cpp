#include <SDL_events.h>
#define GE_USE_SDL
#include "../../core/ecs/GameEngine/GameEngine.hpp"
#include <thread>

// Component Definitions
struct TransformComponent {
    int x, y, width, height;
};

struct VelocityComponent {
    int vx, vy;
};

struct RenderableComponent {
    SDL_Color color;
};

struct PlayerControlComponent {
    SDL_Scancode upKey;
    SDL_Scancode downKey;
};

int main()
{
    core::GameEngine engine{false};
    engine.initWindow({800, 600}, 60, "Pong");

    engine.registry.register_component<TransformComponent>();
    engine.registry.register_component<VelocityComponent>();
    engine.registry.register_component<RenderableComponent>();
    engine.registry.register_component<PlayerControlComponent>();

    engine.registry.add_system<TransformComponent, VelocityComponent>(
        [](core::ecs::Entity, TransformComponent &transform, VelocityComponent &velocity) {
            transform.x += velocity.vx;
            transform.y += velocity.vy;
        });

    engine.registry.add_system<TransformComponent, RenderableComponent>(
        [&engine](core::ecs::Entity, TransformComponent &transform, RenderableComponent &renderable) {
            SDL_Rect rect = {transform.x, transform.y, transform.width, transform.height};
            SDL_SetRenderDrawColor(engine.renderer, renderable.color.r, renderable.color.g, renderable.color.b, renderable.color.a);
            SDL_RenderFillRect(engine.renderer, &rect);
        });

    engine.registry.add_system<TransformComponent, PlayerControlComponent>(
        [](core::ecs::Entity, TransformComponent &transform, PlayerControlComponent &playerControl) {
            const Uint8 *state = SDL_GetKeyboardState(nullptr);
            if (state[playerControl.upKey])
                transform.y -= 5;
            if (state[playerControl.downKey])
                transform.y += 5;
        });

    core::ecs::Entity player1 = engine.registry.spawn_entity();
    engine.registry.add_component<TransformComponent>(player1, {10, 10, 20, 100});
    engine.registry.add_component<VelocityComponent>(player1, {0, 0});
    engine.registry.add_component<RenderableComponent>(player1, {255, 0, 0, 255});
    engine.registry.add_component<PlayerControlComponent>(player1, {SDL_SCANCODE_W, SDL_SCANCODE_S});

    core::ecs::Entity player2 = engine.registry.spawn_entity();
    engine.registry.add_component<TransformComponent>(player2, {770, 10, 20, 100});
    engine.registry.add_component<VelocityComponent>(player2, {0, 0});
    engine.registry.add_component<RenderableComponent>(player2, {0, 0, 255, 255});
    engine.registry.add_component<PlayerControlComponent>(player2, {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN});

    bool isRunning = true;

    while (isRunning) {
        SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 255);
        SDL_RenderClear(engine.renderer);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                isRunning = false;
                break;
            }
        }

        engine.registry.run_system<TransformComponent, VelocityComponent>();
        engine.registry.run_system<TransformComponent, RenderableComponent>();
        engine.registry.run_system<TransformComponent, PlayerControlComponent>();

        SDL_RenderPresent(engine.renderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    SDL_Quit();
    return 0;
}
