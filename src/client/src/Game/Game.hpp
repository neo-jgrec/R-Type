#pragma once

#include "../../../core/ecs/Entity/Entity.hpp"
#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "MusicManager.hpp"

#include <SFML/Graphics.hpp>

class Game {
public:
    Game()
    {
        init();
    }
    ~Game() = default;

    void run();
private:
    void processEvents();
    void update();
    void render();
    void addSystems();
    void init();
    void setMusicVolume(float volume);

    core::ecs::Entity _playerEntity = core::ecs::Entity();
    core::ecs::Entity _enemyEntity = core::ecs::Entity();

    core::GameEngine _gameEngine;

    bool _windowOpen = true;

    static void inputSystem(core::ecs::Registry& registry);
    static void projectileMovementSystem(core::ecs::Registry& registry);
    static void enemyMovementSystem(core::ecs::Registry& registry);

    MusicManager _musicManager;
};
