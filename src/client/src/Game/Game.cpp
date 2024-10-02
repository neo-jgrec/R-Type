#include "Game.hpp"
#include <SFML/System/Time.hpp>
#include "EntityFactory.hpp"
#include "../../../game/Components.hpp"

void Game::init() {
    _gameEngine.window.setFramerateLimit(60);
    _gameEngine.window.setKeyRepeatEnabled(true);

    _gameEngine.registry.register_component<VelocityComponent>();
    _gameEngine.registry.register_component<InputStateComponent>();
    _gameEngine.registry.register_component<HealthComponent>();
    _gameEngine.registry.register_component<ScoreComponent>();
    _gameEngine.registry.register_component<Player>();
    _gameEngine.registry.register_component<Enemy>();
    _gameEngine.registry.register_component<Projectile>();
    _gameEngine.registry.register_component<DamageComponent>();

    _playerEntity = EntityFactory::createPlayer(_gameEngine.registry, sf::Vector2f(100.0f, 100.0f));
    _enemyEntity = EntityFactory::createEnemy(_gameEngine.registry, sf::Vector2f(700.0f, 100.0f));

    inputSystem(_gameEngine.registry);
    projectileMovementSystem(_gameEngine.registry);
    enemyMovementSystem(_gameEngine.registry);
}

void Game::update() {
    _gameEngine.registry.run_system<core::ge::TransformComponent, VelocityComponent, InputStateComponent, Player>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::TransformComponent>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::AnimationComponent>();

    // Sound
    _gameEngine.registry.run_system<core::ge::SoundComponent>();

    // Projectile movement
    _gameEngine.registry.run_system<core::ge::TransformComponent, VelocityComponent, Projectile>();

    // Enemy movement
    _gameEngine.registry.run_system<core::ge::TransformComponent, VelocityComponent, Enemy>();

    // Collision detection
    _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::CollisionComponent>();
}

void Game::render() {
    _gameEngine.window.clear();
    _gameEngine.registry.run_system<core::ge::DrawableComponent>();
    _gameEngine.window.display();
}

void Game::processEvents() {
    sf::Event event{};
    while (_gameEngine.window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _windowOpen = false;
            _gameEngine.window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                _windowOpen = false;
                _gameEngine.window.close();
            }

            auto &inputStateOpt = _gameEngine.registry.get_components<InputStateComponent>()[_playerEntity];
            auto &keyBindingOpt = _gameEngine.registry.get_components<core::ge::KeyBinding>()[_playerEntity];

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
        sf::Time elapsed = _gameEngine.clock.restart();
        _gameEngine.delta_t = elapsed.asSeconds();

        processEvents();
        update();
        render();
    }
}
