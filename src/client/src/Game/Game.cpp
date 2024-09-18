#include "Game.hpp"

void Game::run()
{
    while (_windowOpen) {
        processEvents();
        update();
        render();
    }
}

void Game::addSystems()
{
   // Movement system
    _registry.add_system<Position, Velocity, ClockComponent>(
        [](Position &pos, const Velocity &vel, ClockComponent &) {
            pos.x += vel.dx;
            pos.y += vel.dy;
        }
    );

   _registry.add_system<Drawable, Position>(
        [](Drawable &drawable, const Position &pos) {
            drawable.shape.setPosition(pos.x, pos.y);
        }
    );

    _registry.add_system<Drawable>(
        [this](Drawable &drawable) {
            _window.draw(drawable.shape);
        }
    );

    // Event system
    _registry.add_system<EventComponent>(
        [this](EventComponent &eventComp) {
            while (_window.pollEvent(eventComp.event)) {
                if (eventComp.event.type == sf::Event::Closed) {
                    _windowOpen = false;
                    _window.close();
                }
            }
        }
    );
}

void Game::init()
{
    _window.create(sf::VideoMode(800, 600), "ECS Game");
    _window.setFramerateLimit(60);

    _registry.register_component<Position>();
    _registry.register_component<Velocity>();
    _registry.register_component<Drawable>();
    _registry.register_component<ClockComponent>();
    _registry.register_component<EventComponent>();

    // _playerEntity = _registry.spawn_entity();
    _registry.add_component(_playerEntity, Position{200.0f, 200.0f});
    _registry.add_component(_playerEntity, Velocity{10.0f, 0.0f});
    _registry.add_component(_playerEntity, Drawable{sf::RectangleShape(sf::Vector2f(50.0f, 50.0f))});
    _registry.add_component(_playerEntity, ClockComponent{});

    // _windowEntity = _registry.spawn_entity();
    // _registry.add_component(_window, EventComponent{});

    addSystems();
}

void Game::processEvents()
{
    _registry.run_system<EventComponent>();
}

void Game::update()
{
    _registry.run_system<Position, Velocity, ClockComponent>();
    _registry.run_system<Drawable, Position>();
}

void Game::render()
{
    _window.clear();
    _registry.run_system<Drawable>();
    _window.display();
}