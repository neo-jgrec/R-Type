#pragma once

#include "../../../core/ecs/Registry/Registry.hpp"
#include "../../../core/ecs/Entity/Entity.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include "Components.hpp"

class Game {
public:
    Game(core::ecs::Registry &registry) : _registry(registry), _playerEntity(_registry.spawn_entity())
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
    core::ecs::Entity _playerEntity;
    sf::RenderWindow _window;
    bool _windowOpen = true;
};