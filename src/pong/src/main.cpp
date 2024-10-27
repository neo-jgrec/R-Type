#define GE_USE_SDL
#include "../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../core/ecs/GameEngine/AssetManager.hpp"
#include "../../core/ecs/GameEngine/GameEngineComponents.hpp"
#include "../../core/ecs/Registry/Registry.hpp"

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

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Pong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    core::ecs::Registry registry;

    // Player paddle entities
    auto player1 = registry.spawn_entity();
    registry.emplace_component<TransformComponent>(player1, 50, 250, 20, 100);
    registry.emplace_component<VelocityComponent>(player1, 0, 0);
    registry.emplace_component<RenderableComponent>(player1, SDL_Color{255, 255, 255, 255});
    registry.emplace_component<PlayerControlComponent>(player1, SDL_SCANCODE_W, SDL_SCANCODE_S);

    auto player2 = registry.spawn_entity();
    registry.emplace_component<TransformComponent>(player2, 730, 250, 20, 100);
    registry.emplace_component<VelocityComponent>(player2, 0, 0);
    registry.emplace_component<RenderableComponent>(player2, SDL_Color{255, 255, 255, 255});
    registry.emplace_component<PlayerControlComponent>(player2, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);

    // Ball entity
    auto ball = registry.spawn_entity();
    registry.emplace_component<TransformComponent>(ball, 390, 290, 20, 20);
    registry.emplace_component<VelocityComponent>(ball, 5, 5);
    registry.emplace_component<RenderableComponent>(ball, SDL_Color{255, 255, 255, 255});

    bool quit = false;
    SDL_Event event;

    // Game loop
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Handle player input
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        auto p1_transform = registry.get_component<TransformComponent>(player1);
        auto p1_velocity = registry.get_component<VelocityComponent>(player1);
        auto p1_control = registry.get_component<PlayerControlComponent>(player1);
        p1_velocity->vy = (keystate[p1_control->upKey] ? -5 : 0) + (keystate[p1_control->downKey] ? 5 : 0);

        auto p2_transform = registry.get_component<TransformComponent>(player2);
        auto p2_velocity = registry.get_component<VelocityComponent>(player2);
        auto p2_control = registry.get_component<PlayerControlComponent>(player2);
        p2_velocity->vy = (keystate[p2_control->upKey] ? -5 : 0) + (keystate[p2_control->downKey] ? 5 : 0);

        // Update positions
        for (auto entity : registry.get_entities<TransformComponent, VelocityComponent>()) {
            auto transform = registry.get_component<TransformComponent>(entity);
            auto velocity = registry.get_component<VelocityComponent>(entity);

            transform->x += velocity->vx;
            transform->y += velocity->vy;

            // Wall collision for paddles
            if (entity == player1 || entity == player2) {
                if (transform->y < 0) transform->y = 0;
                if (transform->y + transform->height > 600) transform->y = 600 - transform->height;
            }
            // Ball collision with walls
            else if (entity == ball) {
                if (transform->y <= 0 || transform->y + transform->height >= 600) {
                    velocity->vy = -velocity->vy;
                }
                if (transform->x <= 0 || transform->x + transform->width >= 800) {
                    transform->x = 390; transform->y = 290;
                }
            }
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto entity : registry.get_entities<TransformComponent, RenderableComponent>()) {
            auto transform = registry.get_component<TransformComponent>(entity);
            auto renderable = registry.get_component<RenderableComponent>(entity);
            SDL_Rect rect = { transform->x, transform->y, transform->width, transform->height };
            SDL_SetRenderDrawColor(renderer, renderable->color.r, renderable->color.g, renderable->color.b, renderable->color.a);
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // Roughly 60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
