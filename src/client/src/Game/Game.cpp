#include "Game.hpp"
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "../../../game/Components.hpp"
#include "src/Game/Utils/ClientComponents.hpp"
#include "src/event/EventPool.hpp"
#include <vector>

#include "Scenes.hpp"

void Game::init()
{
    _gameEngine.currentScene = GameState::Loading;

    _gameEngine.window.setFramerateLimit(60);
    _gameEngine.window.setKeyRepeatEnabled(true);

    _gameEngine.assetManager.loadFont("arial", "assets/Fonts/Arial.ttf");
    updateLoadingProgress(10);
    loadAssets();

    updateLoadingProgress(30);
    _gameEngine.musicManager.addMusic("level1", "level1", _gameEngine.assetManager);
    _gameEngine.musicManager.addMusic("menu", "menu", _gameEngine.assetManager);
    _gameEngine.musicManager.setVolume(10.0f);

    updateLoadingProgress(50);
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

    updateLoadingProgress(70);
    inputSystem(*this);
    playerMovementSystem(_gameEngine.registry);
    viewSystem(*this);
    eventSystem(*this);

    _viewEntity = _gameEngine.registry.spawn_entity();
    _gameEngine.registry.add_component(_viewEntity, ViewComponent{_gameEngine.window.getDefaultView()});

    updateLoadingProgress(80);
    _networkingService.init();
    setHandlers();
    networkingService.run();

    core::ecs::Entity EventEntity = _gameEngine.registry.spawn_entity();
    _gameEngine.registry.add_component(EventEntity, EventComponent{});
    eventSystem(_gameEngine.registry);

    updateLoadingProgress(100);
}

void Game::loadAssets()
{
    try {
        _gameEngine.assetManager.loadTexture("enemie1", "assets/Enemies/enemie1.png");
        updateLoadingProgress(12);
        _gameEngine.assetManager.loadTexture("player", "assets/player_sprite.png");
        updateLoadingProgress(13);
        _gameEngine.assetManager.loadSound("shooting", "assets/shooting_sound.ogg");
        updateLoadingProgress(14);
        _gameEngine.assetManager.loadTexture("player_projectile", "assets/player_projectile.png");
        updateLoadingProgress(15);
        _gameEngine.assetManager.loadSound("missile_sound", "assets/missile_sound.ogg");
        updateLoadingProgress(16);
        _gameEngine.assetManager.loadTexture("player_missile", "assets/player_missile.png");
        updateLoadingProgress(17);
        _gameEngine.assetManager.loadTexture("enemie1", "assets/Enemies/enemie1.png");
        updateLoadingProgress(18);
        _gameEngine.assetManager.loadFont("arial", "assets/Fonts/Arial.ttf");
        updateLoadingProgress(19);
        _gameEngine.assetManager.loadTexture("background", "assets/background.png");
        updateLoadingProgress(20);
        _gameEngine.assetManager.loadTexture("logo", "assets/logo.png");
        updateLoadingProgress(21);
        _gameEngine.assetManager.loadTexture("player_anim", "assets/Player/missile_charging.png");
        updateLoadingProgress(22);
        _gameEngine.assetManager.loadMusic("level1", "assets/music/level1.ogg");
        updateLoadingProgress(23);
        _gameEngine.assetManager.loadMusic("menu", "assets/music/menu.ogg");
        updateLoadingProgress(24);
    } catch (const std::runtime_error &e) {
        std::cerr << "Error loading assets: " << e.what() << std::endl;
    }
}

void Game::sound()
{
    _gameEngine.registry.run_system<core::ge::SoundComponent>();
}

void Game::render()
{
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::TransformComponent>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::AnimationComponent>();

    _gameEngine.window.clear();
    _gameEngine.registry.run_system<ViewComponent>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent>();
    _gameEngine.registry.run_system<core::ge::DrawableComponent, core::ge::DisabledComponent>();
    _gameEngine.registry.run_system<core::ge::TextComponent>();
    _gameEngine.registry.run_system<core::ge::SliderComponent>();
    _gameEngine.registry.run_system<core::ge::TextInputComponent, core::ge::DrawableComponent, core::ge::TextComponent>();
    _gameEngine.window.display();
}

void Game::processEvents()
{
    sf::Event event{};
    while (_gameEngine.window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _windowOpen = false;
            _gameEngine.window.close();
            _networkingService.stop();
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

        if (keyToUpdate.has_value() && event.type == sf::Event::KeyPressed) {
            sf::Keyboard::Key key = event.key.code;
            core::ecs::Entity entity = keyBindingTexts[keyToUpdate.value()];
            auto text = _gameEngine.registry.get_component<core::ge::TextComponent>(entity);

            if (keyToUpdate.value() == "Move Up") {
                _gameEngine.keyBindingsConfig.moveUpKey = key;
                // text->text.setString("Move Up: " + Menus::keyToString(key));
            } else if (keyToUpdate.value() == "Move Down"){
                _gameEngine.keyBindingsConfig.moveDownKey = key;
                // text->text.setString("Move Down: " + Menus::keyToString(key));
            } else if (keyToUpdate.value() == "Move Left"){
                _gameEngine.keyBindingsConfig.moveLeftKey = key;
                // text->text.setString("Move Left: " + Menus::keyToString(key));
            } else if (keyToUpdate.value() == "Move Right"){
                _gameEngine.keyBindingsConfig.moveRightKey = key;
                // text->text.setString("Move Right: " + Menus::keyToString(key));
            } else if (keyToUpdate.value() == "Shoot") {
                _gameEngine.keyBindingsConfig.fireKey = key;
                // text->text.setString("Shoot: " + Menus::keyToString(key));
            }
            keyToUpdate.reset();
        }
    }
}

int Game::assignColor() {
    if (availableColors.empty()) {
        std::cerr << "No available colors left!" << std::endl;
        return -1;
    }

    const int color = availableColors.back();
    availableColors.pop_back();
    return color;
}

void Game::releaseColor(const int color) {
    availableColors.push_back(color);
}

void Game::run() {
    _gameEngine.musicManager.playMusic("level1");
    _networkingService.run();

    // Load map on the game scene only set at half the screen size and other scale are biger
    parseMap(*this, "./JY_map.json", _gameEngine.window);
    Scenes::loadMainMenu(*this);
    while (_windowOpen) {
        sf::Time elapsed = _gameEngine.clock.restart();
        _gameEngine.delta_t = elapsed.asSeconds();

        processEvents();
        switch (_gameEngine.currentScene) {
            case MainMenu:
                Scenes::updateMainMenu(*this);
                break;
            case Playing:
                Scenes::updateGame(*this);
                break;
            case Loading:
                renderLoadingScreen();
                break;
            default:
                break;
        }
        sound();
        render();
    }
}

void Game::renderLoadingScreen()
{
    _gameEngine.window.clear(sf::Color(30, 30, 30));

    sf::Text loadingText;
    auto font = _gameEngine.assetManager.getFont("arial");
    loadingText.setFont(font);
    loadingText.setString("Loading...");
    loadingText.setCharacterSize(52);
    loadingText.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = loadingText.getLocalBounds();
    loadingText.setOrigin(
        textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f
    );
    loadingText.setPosition(
        static_cast<float>(_gameEngine.window.getSize().x) / 2.0f,
        static_cast<float>(_gameEngine.window.getSize().y) / 2.0f
    );

    sf::RectangleShape loadingBar;
    sf::Vector2f barSize(400.0f, 20.0f);
    loadingBar.setSize(barSize);
    loadingBar.setFillColor(sf::Color(100, 100, 100));
    loadingBar.setOutlineColor(sf::Color::White);
    loadingBar.setOutlineThickness(2);
    loadingBar.setPosition((
        static_cast<float>(_gameEngine.window.getSize().x) - barSize.x) / 2.0f,
        static_cast<float>(_gameEngine.window.getSize().y) / 2.0f + 50.0f
    );

    float progress = _loadingProgress / 100.0f;
    sf::RectangleShape progressBar;
    progressBar.setSize(sf::Vector2f(barSize.x * progress, barSize.y));
    progressBar.setFillColor(sf::Color(0, 255, 0));
    progressBar.setPosition(loadingBar.getPosition());

    _gameEngine.window.draw(loadingBar);
    _gameEngine.window.draw(progressBar);
    _gameEngine.window.draw(loadingText);
    _gameEngine.window.display();
}

void Game::updateLoadingProgress(float progress)
{
    _loadingProgress = progress;
    renderLoadingScreen();
}
