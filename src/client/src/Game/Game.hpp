#pragma once

#include "../../../core/ecs/Entity/Entity.hpp"
#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "MusicManager.hpp"

class Game {
public:
    Game()
    {
        init();
    }
    ~Game() = default;

    void run();

    enum class GameState {
        MainMenu = 0,
        Playing = 1,
        GameOver = 2
    };
private:
    void processEvents();
    void update();
    void render();
    void init();
    void setMusicVolume(float volume);
    int assignColor();
    void releaseColor(int color);

    core::ecs::Entity _playerEntity = core::ecs::Entity();
    core::ecs::Entity _enemyEntity = core::ecs::Entity();

    core::GameEngine _gameEngine;

    bool _windowOpen = true;

    GameState _currentState = GameState::MainMenu;

    static void inputSystem(core::ecs::Registry& registry);
    static void projectileMovementSystem(core::ecs::Registry& registry);
    static void enemyMovementSystem(core::ecs::Registry& registry);

    MusicManager _musicManager;
    std::vector<int> availableColors = {0, 1, 2, 3, 4};
};
