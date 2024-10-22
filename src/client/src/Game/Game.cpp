#include "Game.hpp"
#include <SFML/System/Time.hpp>
#include <iostream>
#include "EntityFactory.hpp"
#include "../../../game/Components.hpp"
#include "src/Game/Utils/ClientComponents.hpp"
#include "src/event/EventPool.hpp"
#include <iostream>
#include <vector>
#include "../../../game/RequestType.hpp"

void Game::init()
{
    _gameEngine.currentScene = static_cast<int>(GameState::MainMenu);

    _gameEngine.window.setFramerateLimit(60);
    _gameEngine.window.setKeyRepeatEnabled(true);

    _gameEngine.musicManager.loadMusic("level1", "assets/music/level1.ogg");
    _gameEngine.musicManager.setVolume(10.0f);
    // TODO: load every level music
    _gameEngine.musicManager.playMusic("level1");

    _gameEngine.registry.register_component<VelocityComponent>();
    _gameEngine.registry.register_component<InputStateComponent>();
    _gameEngine.registry.register_component<HealthComponent>();
    _gameEngine.registry.register_component<ScoreComponent>();
    _gameEngine.registry.register_component<Player>();
    _gameEngine.registry.register_component<PlayerAnim>();
    _gameEngine.registry.register_component<Enemy>();
    _gameEngine.registry.register_component<Projectile>();
    _gameEngine.registry.register_component<Missile>();
    _gameEngine.registry.register_component<ShootCounterComponent>();
    _gameEngine.registry.register_component<DamageComponent>();
    _gameEngine.registry.register_component<PlayerColorComponent>();
    _gameEngine.registry.register_component<ViewComponent>();
    _gameEngine.registry.register_component<EventComponent>();
    _gameEngine.registry.register_component<TileComponent>();

    // TODO: implement a way to load maps at runtimes dynamically
    parseMap(_gameEngine.registry, "./JY_map.json", _gameEngine.window);

    // _enemyEntity = EntityFactory::createEnemy(_gameEngine.registry, sf::Vector2f(700.0f, 100.0f), gameScale);

    initMainMenu();

    inputSystem(_gameEngine.registry);
    projectileMovementSystem(_gameEngine.registry);
    enemyMovementSystem(_gameEngine.registry);
    viewSystem(_gameEngine.registry);

    _viewEntity = _gameEngine.registry.spawn_entity();
    _gameEngine.registry.add_component(_viewEntity, ViewComponent{_gameEngine.window.getDefaultView()});
    _gameEngine.registry.add_component(_viewEntity, core::ge::SceneComponent{static_cast<int>(GameState::Playing)});

    networkingService.init();
    setHandlers();
    networkingService.run();

    core::ecs::Entity EventEntity = _gameEngine.registry.spawn_entity();
    _gameEngine.registry.add_component(EventEntity, EventComponent{});
    eventSystem(_gameEngine.registry);
}

void Game::initMainMenu()
{
    sf::Vector2u windowSize = _gameEngine.window.getSize();
    float centerX = static_cast<float>(windowSize.x) / 2.0f;
    float centerY = static_cast<float>(windowSize.y) / 2.0f;

    sf::Vector2f buttonSize(200.0f, 50.0f);
    float buttonSpacing = 20.0f;

    EntityFactory::createImage(_gameEngine.registry,
        sf::Vector2f(0.0f, 0.0f),
        sf::Vector2f(static_cast<float>(_gameEngine.window.getSize().x), static_cast<float>(windowSize.y)),
        "assets/background.png",
        static_cast<int>(GameState::MainMenu)
    );

    EntityFactory::createImage(_gameEngine.registry,
        sf::Vector2f(centerX - 500.0f, centerY - 400.0f),
        sf::Vector2f(1000.0f, 300.0f),
        "assets/logo.png",
        static_cast<int>(GameState::MainMenu)
    );

    EntityFactory::createButton(_gameEngine.registry,
        sf::Vector2f(centerX - buttonSize.x / 2, centerY - buttonSize.y - buttonSpacing),
        buttonSize,
        "Start Game",
        [this]() {
            _gameEngine.currentScene = static_cast<int>(GameState::Playing);
            playerConnectionHeader = networkingService.sendRequest("127.0.0.1", 1111, PlayerConnect, {});
            networkingService.sendRequest("127.0.0.1", 1111, GameStart, {});
        },
        static_cast<int>(GameState::MainMenu)
    );

    EntityFactory::createButton(_gameEngine.registry,
        sf::Vector2f(centerX - buttonSize.x / 2, centerY),
        buttonSize,
        "Options",
        []() { },
        static_cast<int>(GameState::MainMenu)
    );

    EntityFactory::createButton(_gameEngine.registry,
        sf::Vector2f(centerX - buttonSize.x / 2, centerY + buttonSize.y + buttonSpacing),
        buttonSize,
        "Quit",
        [this]() {
            _windowOpen = false;
            networkingService.stop();
        },
        static_cast<int>(GameState::MainMenu)
    );

    EntityFactory::createTextInput(_gameEngine.registry,
        sf::Vector2f(centerX - buttonSize.x / 2, centerY + 2 * (buttonSize.y + (buttonSpacing += 10.0f))),
        buttonSize,
        "Enter your name",
        static_cast<int>(GameState::MainMenu)
    );

    EntityFactory::createSlider(_gameEngine.registry,
        sf::Vector2f(centerX - buttonSize.x / 2, centerY + 3 * (buttonSize.y + (buttonSpacing += 10.0f))),
        sf::Vector2f(200.0f, 10.0f),
        "Volume",
        [this](float value) { _gameEngine.musicManager.setVolume(value); },
        static_cast<int>(GameState::MainMenu),
        _gameEngine.musicManager.getVolume()
    );
}

void Game::update()
{
    // events
    _gameEngine.registry.run_system<EventComponent>();

    _gameEngine.registry.run_system<core::ge::TransformComponent, VelocityComponent, InputStateComponent, ShootCounterComponent, Player, core::ge::AnimationComponent>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::TransformComponent>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::AnimationComponent>();

    // Sound
    _gameEngine.registry.run_system<core::ge::SoundComponent>();

    // Projectile movement
    _gameEngine.registry.run_system<core::ge::TransformComponent, VelocityComponent, Projectile>();

    // Enemy movement
    if (_gameEngine.currentScene == static_cast<int>(GameState::Playing))
        _gameEngine.registry.run_system<core::ge::TransformComponent, VelocityComponent, Enemy>();

    // Collision detection
    _gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::CollisionComponent, core::ge::SceneComponent>();

    // Clickable
    _gameEngine.registry.run_system<core::ge::ClickableComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent, core::ge::TransformComponent>();

    // TextInput
    _gameEngine.registry.run_system<core::ge::TextInputComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent>();

    // View
    _gameEngine.registry.run_system<ViewComponent, core::ge::SceneComponent>();
}

void Game::render()
{
    _gameEngine.window.clear();
    _gameEngine.registry.run_system<ViewComponent, core::ge::SceneComponent>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::SceneComponent>();
    _gameEngine.registry.run_system<core::ge::TextComponent, core::ge::SceneComponent>();
    _gameEngine.registry.run_system<core::ge::SliderComponent, core::ge::SceneComponent>();
    _gameEngine.window.display();
}

void Game::processEvents()
{
    sf::Event event{};
    while (_gameEngine.window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _windowOpen = false;
            _gameEngine.window.close();
            networkingService.stop();
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
            bool isPressed = (event.type == sf::Event::KeyPressed);

            if (event.key.code == sf::Keyboard::Escape && isPressed) {
                _windowOpen = false;
                _gameEngine.window.close();
            }

            auto playerEntities = _gameEngine.registry.get_entities<Player, InputStateComponent>();

            if (!playerEntities.empty()) {
                auto playerEntity = playerEntities[0];

                auto &inputStateOpt = _gameEngine.registry.get_components<InputStateComponent>()[playerEntity];
                auto &keyBindingOpt = _gameEngine.registry.get_components<core::ge::KeyBinding>()[playerEntity];

                if (inputStateOpt.has_value() && keyBindingOpt.has_value()) {
                    auto &keyBinding = *keyBindingOpt.value();
                    auto &inputState = *inputStateOpt.value();

                    auto set_input_state = [&event, isPressed](auto key, bool &state) {
                        if (event.key.code == key) {
                            state = isPressed;
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

        if (event.type == sf::Event::TextEntered) {
            auto textInputEntities = _gameEngine.registry.get_entities<core::ge::TextInputComponent>();
            for (auto entity : textInputEntities) {
                auto textInput = _gameEngine.registry.get_component<core::ge::TextInputComponent>(entity);
                if (textInput->isActive) {
                    if (event.text.unicode == '\b' && !textInput->text.empty()) {
                        textInput->text.pop_back();
                    } else if (event.text.unicode < 128 && textInput->text.size() < textInput->maxLength) {
                        textInput->text += static_cast<char>(event.text.unicode);
                    }
                }
            }
        }

        if (event.type == sf::Event::Resized) {
            gameScale.x = static_cast<float>(_gameEngine.window.getSize().x) / 1920.0f;
            gameScale.y = static_cast<float>(_gameEngine.window.getSize().y) / 1080.0f;
            auto entities = _gameEngine.registry.get_entities<core::ge::TransformComponent>();
            for (auto entity : entities) {
                auto &transform = _gameEngine.registry.get_components<core::ge::DrawableComponent>()[entity];
                if (transform.has_value()) {
                    transform->get()->shape.setScale(gameScale);
                }
            }
        }
    }
}

int Game::assignColor() {
    if (!availableColors.empty()) {
        int color = availableColors.back();
        availableColors.pop_back();
        return color;
    } else {
        std::cerr << "No available colors left!" << std::endl;
        return -1;
    }
}

void Game::releaseColor(int color) {
    availableColors.push_back(color);
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
