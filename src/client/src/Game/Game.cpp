#include "Game.hpp"
#include <iostream>
#include "Components.hpp"

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
    _registry.add_system<Position, Velocity, InputStateComponent>(
        [](Position &pos, const Velocity &vel, InputStateComponent &input)
        {
            if (input.up) {
                pos.y -= vel.dy;
                input.up = false;
            }
            if (input.down) {
                pos.y += vel.dy;
                input.down = false;
            }
            if (input.left) {
                pos.x -= vel.dx;
                input.left = false;
            }
            if (input.right) {
                pos.x += vel.dx;
                input.right = false;
            }
        });

    _registry.add_system<Drawable, Position>([](Drawable &drawable, const Position &pos)
                                             { drawable.shape.setPosition(pos.x, pos.y); });

    _registry.add_system<Drawable>([this](Drawable &drawable) { _window.draw(drawable.shape); });
}

void Game::init()
{
    _window.create(sf::VideoMode(800, 600), "R-Type");
    _window.setFramerateLimit(60);
    _window.setKeyRepeatEnabled(true);

    _registry.register_component<Position>();
    _registry.register_component<Velocity>();
    _registry.register_component<Drawable>();
    _registry.register_component<ClockComponent>();
    _registry.register_component<KeyBinding>();
    _registry.register_component<InputStateComponent>();

    _registry.add_component(_playerEntity, Position{200.0f, 200.0f});
    _registry.add_component(_playerEntity, Velocity{10.0f, 10.0f});
    _registry.add_component(_playerEntity, Drawable{sf::RectangleShape(sf::Vector2f(50.0f, 50.0f))});
    _registry.add_component(_playerEntity, ClockComponent{});
    _registry.add_component(_playerEntity, KeyBinding{});
    _registry.add_component(_playerEntity, InputStateComponent{});


    addSystems();
}

void Game::processEvents()
{
    sf::Event event{};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _windowOpen = false;
            _window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                _windowOpen = false;
                _window.close();
            }

            auto &inputStateOpt = _registry.get_components<InputStateComponent>()[_playerEntity];
            auto &keyBindingOpt = _registry.get_components<KeyBinding>()[_playerEntity];

            if (inputStateOpt.has_value()) {
                auto &keyBinding = *keyBindingOpt.value();
                auto &inputState = *inputStateOpt.value();

                auto set_input_state = [&inputState, &event](auto key, bool &state) {
                    if (event.key.code == key) {
                        state = true;
                    }
                };

                set_input_state(keyBinding.moveUpKey, inputState.up);
                set_input_state(keyBinding.moveDownKey, inputState.down);
                set_input_state(keyBinding.moveLeftKey, inputState.left);
                set_input_state(keyBinding.moveRightKey, inputState.right);

            }
        }
    }
}


void Game::update()
{
    _registry.run_system<Position, Velocity, InputStateComponent>();
    _registry.run_system<Drawable, Position>();
}

void Game::render()
{
    _window.clear();
    _registry.run_system<Drawable>();
    _window.display();
}
