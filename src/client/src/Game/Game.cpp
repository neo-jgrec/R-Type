#include "Game.hpp"
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include "EntityFactory.hpp"
#include "../../../game/Components.hpp"
#include "src/Game/Utils/ClientComponents.hpp"
#include "src/event/EventPool.hpp"
#include <iostream>
#include <ostream>
#include <vector>
#include "../../../game/RequestType.hpp"

void Game::init()
{
    _gameEngine.currentScene = static_cast<int>(GameState::MainMenu);

    _gameEngine.window.setFramerateLimit(60);
    _gameEngine.window.setKeyRepeatEnabled(true);

    loadAssets();
    _configManager.parse("assets/Data/config.json");
    initWindow();

    _gameEngine.musicManager.loadMusic("level1", "assets/music/level1.ogg");
    _gameEngine.musicManager.setVolume(10.0f);
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

    menus.initMainMenu();
    menus.initRoomMenu();

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

void Game::initWindow()
{
    sf::VideoMode videoMode(
        _configManager.getValue<int>("/view/size/x"),
        _configManager.getValue<int>("/view/size/y")
    );
    _gameEngine.initWindow(videoMode, 60, "R-Type");
}

void Game::loadAssets()
{
    try {
        _gameEngine.assetManager.loadTexture("enemie1", "assets/Enemies/enemie1.png");
        _gameEngine.assetManager.loadTexture("player", "assets/player_sprite.png");
        _gameEngine.assetManager.loadSound("shooting", "assets/shooting_sound.ogg");
        _gameEngine.assetManager.loadTexture("player_projectile", "assets/player_projectile.png");
        _gameEngine.assetManager.loadSound("missile_sound", "assets/missile_sound.ogg");
        _gameEngine.assetManager.loadTexture("player_missile", "assets/player_missile.png");
        _gameEngine.assetManager.loadTexture("enemie1", "assets/Enemies/enemie1.png");
        _gameEngine.assetManager.loadFont("arial", "assets/Fonts/Arial.ttf");
        _gameEngine.assetManager.loadTexture("background", "assets/background.png");
        _gameEngine.assetManager.loadTexture("logo", "assets/logo.png");
        _gameEngine.assetManager.loadTexture("player_anim", "assets/Player/missile_charging.png");
    } catch (const std::runtime_error &e) {
        std::cerr << "Error loading assets: " << e.what() << std::endl;
    }
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

    // View
    _gameEngine.registry.run_system<ViewComponent, core::ge::SceneComponent>();
}

void Game::render()
{
    _gameEngine.window.clear();
    _gameEngine.registry.run_system<ViewComponent, core::ge::SceneComponent>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::SceneComponent>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::SceneComponent, core::ge::DisabledComponent>();
    _gameEngine.registry.run_system<core::ge::TextComponent, core::ge::SceneComponent>();
    _gameEngine.registry.run_system<core::ge::SliderComponent, core::ge::SceneComponent>();
    _gameEngine.registry.run_system<core::ge::TextInputComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent>();
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
                    std::string value = sf::String(textInput->text.getString()).toAnsiString();
                    if (event.text.unicode == '\b' && !textInput->text.getString().isEmpty()) {
                        value.pop_back();
                    } else if (event.text.unicode < 128 && value.size() < textInput->maxLength) {
                        value += static_cast<char>(event.text.unicode);
                    }
                    textInput->text.setString(value);
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
