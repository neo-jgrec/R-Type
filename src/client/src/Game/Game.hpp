#pragma once

#include <SFML/System/Vector2.hpp>
#include "../../../core/ecs/Entity/Entity.hpp"
#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "MusicManager.hpp"

struct Tile {
    core::ecs::Entity entity;
    sf::Vector2f position;
    bool isDestructible;
};

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
    void initMainMenu();

    core::ecs::Entity _playerEntity = core::ecs::Entity();
    core::ecs::Entity _enemyEntity = core::ecs::Entity();
    core::ecs::Entity _viewEntity = core::ecs::Entity();

    core::GameEngine _gameEngine;

    bool _windowOpen = true;
    sf::Vector2f gameScale = sf::Vector2f(1.0f, 1.0f);

    GameState _currentState = GameState::MainMenu;

    void inputSystem(core::ecs::Registry& registry);
    void projectileMovementSystem(core::ecs::Registry& registry) const;
    void enemyMovementSystem(core::ecs::Registry& registry) const;
    void moveWindowViewSystem(core::ecs::Registry& registry);

    MusicManager _musicManager;
    std::vector<int> availableColors = {0, 1, 2, 3, 4};

    std::vector<std::vector<Tile>> _tileMap;
    void parseMap(core::ecs::Registry& registry, const std::string& mapFilePath, sf::RenderWindow& window);
};
