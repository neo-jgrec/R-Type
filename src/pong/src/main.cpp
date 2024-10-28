#include <SDL_events.h>
#define GE_USE_SDL
#include "../../core/ecs/GameEngine/GameEngine.hpp"
#include <thread>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Custom components
struct VelocityComponent {
    int vx, vy;
};

struct PlayerControlComponent {
    SDL_Scancode upKey;
    SDL_Scancode downKey;
};

struct PlayerScoreComponent {
    size_t score = 0;
};

// Mask used for collision detection (instead of using non human easy to read numbers)
struct CollisonMasks {
    static const int PLAYER = 0b00001;
    static const int BALL = 0b00010;
};

int main()
{
    core::GameEngine engine{false};
    engine.initWindow({SCREEN_WIDTH, SCREEN_HEIGHT}, 60, "Pong");

    engine.currentScene = 0;

    engine.assetManager.loadTexture("ball", "assets/logo.png");
    engine.assetManager.loadTexture("player", "assets/pong/white.png");

    engine.registry.register_component<VelocityComponent>();
    engine.registry.register_component<PlayerControlComponent>();

    engine.registry.add_system<core::ge::TransformComponent, VelocityComponent, core::ge::DrawableComponent>(
        [](core::ecs::Entity, core::ge::TransformComponent &transform, VelocityComponent &velocity, core::ge::DrawableComponent &drawable) {
            transform.position.x += static_cast<float>(velocity.vx);
            transform.position.y += static_cast<float>(velocity.vy);
            drawable.shape.x = static_cast<int>(transform.position.x);
            drawable.shape.y = static_cast<int>(transform.position.y);
        });

    engine.registry.add_system<core::ge::TransformComponent, PlayerControlComponent, core::ge::DrawableComponent>(
        [](core::ecs::Entity, core::ge::TransformComponent &transform, PlayerControlComponent &playerControl, core::ge::DrawableComponent &drawable) {
            const Uint8 *state = SDL_GetKeyboardState(nullptr);
            if (state[playerControl.upKey] && transform.position.y > 0) {
                transform.position.y -= 5;
                drawable.shape.y -= 5;
            }
            if (state[playerControl.downKey] && transform.position.y < SCREEN_HEIGHT - transform.size.y) {
                transform.position.y += 5;
                drawable.shape.y += 5;
            }
        });

    core::ecs::Entity player1 = engine.registry.spawn_entity();
    engine.registry.add_component<core::ge::TransformComponent>(player1, {
        .position = {10, 10},
        .size = {20, 100},
        .scale = {1, 1},
        .rotation = 0,
    });
    engine.registry.add_component<VelocityComponent>(player1, {0, 0});
    engine.registry.add_component<core::ge::DrawableComponent>(player1, {{10, 10, 20, 100}, engine.assetManager.getTexture("player")});
    engine.registry.add_component<PlayerControlComponent>(player1, {SDL_SCANCODE_W, SDL_SCANCODE_S});
    engine.registry.add_component(player1, core::ge::CollisionComponent{CollisonMasks::BALL, {sf::FloatRect(10, 10, 20, 100)}, {
        {0b00001, [&](const core::ecs::Entity self, const core::ecs::Entity) {
            auto velocity = engine.registry.get_component<VelocityComponent>(self);
            velocity->vy = 0;
        }},
    }});
    engine.registry.add_component(player1, core::ge::SceneComponent{0});

    core::ecs::Entity player2 = engine.registry.spawn_entity();
    engine.registry.add_component<core::ge::TransformComponent>(player2, {
        .position = {770, 10},
        .size = {20, 100},
        .scale = {1, 1},
        .rotation = 0,
    });
    engine.registry.add_component<VelocityComponent>(player2, {0, 0});
    engine.registry.add_component<core::ge::DrawableComponent>(player2, {{770, 10, 20, 100}, engine.assetManager.getTexture("player")});
    engine.registry.add_component<PlayerControlComponent>(player2, {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN});
    engine.registry.add_component(player2, core::ge::CollisionComponent{CollisonMasks::BALL, {sf::FloatRect(770, 10, 20, 100)}, {
        {0b00001, [&](const core::ecs::Entity self, const core::ecs::Entity) {
            auto velocity = engine.registry.get_component<VelocityComponent>(self);
            velocity->vy = 0;
        }},
    }});
    engine.registry.add_component(player2, core::ge::SceneComponent{0});

    core::ecs::Entity ball = engine.registry.spawn_entity();
    engine.registry.add_component<core::ge::TransformComponent>(ball, {
        .position = {400, 300},
        .size = {20, 20},
        .scale = {1, 1},
        .rotation = 0,
    });
    engine.registry.add_component<VelocityComponent>(ball, {2, 2});
    engine.registry.add_component<core::ge::DrawableComponent>(ball, {{400, 300, 100, 100}, engine.assetManager.getTexture("ball")});
    engine.registry.add_component(ball, core::ge::CollisionComponent{CollisonMasks::PLAYER, {sf::FloatRect(400, 300, 100, 100)}, {
        {CollisonMasks::BALL, [&](const core::ecs::Entity self, const core::ecs::Entity) {
            auto velocity = engine.registry.get_component<VelocityComponent>(self);
            velocity->vx = -velocity->vx;
        }},
        {CollisonMasks::PLAYER, [&](const core::ecs::Entity self, const core::ecs::Entity) {
            auto velocity = engine.registry.get_component<VelocityComponent>(self);
            velocity->vy = -velocity->vy;
        }},
    }});
    engine.registry.add_component(ball, core::ge::SceneComponent{0});

    bool isRunning = true;

    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                isRunning = false;
                break;
            }
        }

        engine.registry.run_system<core::ge::TransformComponent, VelocityComponent, core::ge::DrawableComponent>();
        engine.registry.run_system<core::ge::TransformComponent, PlayerControlComponent, core::ge::DrawableComponent>();
        engine.registry.run_system<core::ge::DrawableComponent, core::ge::SceneComponent>();

        // RENDER
        SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 255);
        SDL_RenderClear(engine.renderer);

        engine.registry.run_system<core::ge::DrawableComponent, core::ge::SceneComponent>();

        SDL_RenderPresent(engine.renderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    SDL_Quit();
    return 0;
}
