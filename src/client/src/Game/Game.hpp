#pragma once

#include "../../../core/ecs/Registry/Registry.hpp"
#include "../../../core/ecs/Entity/Entity.hpp"
#include "Globals.hpp"
#include <SFML/Graphics.hpp>

class Game {
public:
    Game(core::ecs::Registry &registry) : _registry(registry)
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

    core::ecs::Registry _registry;
    core::ecs::Entity _playerEntity = core::ecs::Entity();
    core::ecs::Entity _player2Entity = core::ecs::Entity();
    sf::RenderWindow _window;
    sf::Clock _clock;
    bool _windowOpen = true;
};