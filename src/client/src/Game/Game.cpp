#include "Game.hpp"
#include <SFML/System/Time.hpp>
#include "EntityFactory.hpp"
#include "Systems.hpp"
#include "Components.hpp"

void Game::init() {
    _window.create(sf::VideoMode(800, 600), "R-Type");
    _window.setFramerateLimit(60);
    _window.setKeyRepeatEnabled(true);

    _registry.register_component<TransformComponent>();
    _registry.register_component<VelocityComponent>();
    _registry.register_component<DrawableComponent>();
    _registry.register_component<InputStateComponent>();
    _registry.register_component<KeyBinding>();
    _registry.register_component<HealthComponent>();
    _registry.register_component<ScoreComponent>();
    _registry.register_component<AnimationComponent>();
    _registry.register_component<TextureComponent>();
    _registry.register_component<Player>();
    _registry.register_component<Projectile>();
    _registry.register_component<DamageComponent>();

    _playerEntity = EntityFactory::createPlayer(_registry);

    Systems::positionSystem(_registry);
    Systems::movementSystem(_registry);
    Systems::renderSystem(_registry, _window);
    Systems::animationSystem(_registry);
    Systems::projectileMovementSystem(_registry);
}

void Game::update() {
    _registry.run_system<TransformComponent, VelocityComponent, InputStateComponent, Player>();
    _registry.run_system<DrawableComponent, TransformComponent>();
    _registry.run_system<DrawableComponent, AnimationComponent>();

    // Projectile movement
    _registry.run_system<TransformComponent, VelocityComponent, Projectile>();
}

void Game::render() {
    _window.clear();
    _registry.run_system<DrawableComponent>();
    _window.display();
}

void Game::processEvents() {
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

                auto set_input_state = [&event](auto key, bool &state) {
                    if (event.key.code == key) {
                        state = true;
                    }
                };

                set_input_state(keyBinding.moveUpKey, inputState.up);
                set_input_state(keyBinding.moveDownKey, inputState.down);
                set_input_state(keyBinding.moveLeftKey, inputState.left);
                set_input_state(keyBinding.moveRightKey, inputState.right);
                set_input_state(keyBinding.fireKey, inputState.fire);
            }
        }
    }
}

void Game::run() {
    while (_windowOpen) {
        sf::Time elapsed = _clock.restart();
        DELTA_T = elapsed.asSeconds();

        processEvents();
        update();
        render();
    }
}

