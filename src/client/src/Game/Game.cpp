#include "Game.hpp"

#include <vector>
#include <iostream>
#include <SFML/System/Time.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Scenes.hpp"
#include "Systems.hpp"
#include "src/event/EventPool.hpp"
#include "src/Game/Utils/ClientComponents.hpp"
#include "../../../game/Components.hpp"


std::string Game::keyToString(const sf::Keyboard::Key key)
{
    static const std::unordered_map<sf::Keyboard::Key, std::string> keyMap = {
        {sf::Keyboard::A, "A"}, {sf::Keyboard::B, "B"}, {sf::Keyboard::C, "C"},
        {sf::Keyboard::D, "D"}, {sf::Keyboard::E, "E"}, {sf::Keyboard::F, "F"},
        {sf::Keyboard::G, "G"}, {sf::Keyboard::H, "H"}, {sf::Keyboard::I, "I"},
        {sf::Keyboard::J, "J"}, {sf::Keyboard::K, "K"}, {sf::Keyboard::L, "L"},
        {sf::Keyboard::M, "M"}, {sf::Keyboard::N, "N"}, {sf::Keyboard::O, "O"},
        {sf::Keyboard::P, "P"}, {sf::Keyboard::Q, "Q"}, {sf::Keyboard::R, "R"},
        {sf::Keyboard::S, "S"}, {sf::Keyboard::T, "T"}, {sf::Keyboard::U, "U"},
        {sf::Keyboard::V, "V"}, {sf::Keyboard::W, "W"}, {sf::Keyboard::X, "X"},
        {sf::Keyboard::Y, "Y"}, {sf::Keyboard::Z, "Z"},

        {sf::Keyboard::Num0, "0"}, {sf::Keyboard::Num1, "1"}, {sf::Keyboard::Num2, "2"},
        {sf::Keyboard::Num3, "3"}, {sf::Keyboard::Num4, "4"}, {sf::Keyboard::Num5, "5"},
        {sf::Keyboard::Num6, "6"}, {sf::Keyboard::Num7, "7"}, {sf::Keyboard::Num8, "8"},
        {sf::Keyboard::Num9, "9"},

        {sf::Keyboard::Escape, "Escape"}, {sf::Keyboard::LControl, "Left Control"},
        {sf::Keyboard::LShift, "Left Shift"}, {sf::Keyboard::LAlt, "Left Alt"},
        {sf::Keyboard::LSystem, "Left System"}, {sf::Keyboard::RControl, "Right Control"},
        {sf::Keyboard::RShift, "Right Shift"}, {sf::Keyboard::RAlt, "Right Alt"},
        {sf::Keyboard::RSystem, "Right System"}, {sf::Keyboard::Menu, "Menu"},

        {sf::Keyboard::LBracket, "["}, {sf::Keyboard::RBracket, "]"}, {sf::Keyboard::SemiColon, ";"},
        {sf::Keyboard::Comma, ","}, {sf::Keyboard::Period, "."}, {sf::Keyboard::Quote, "'"},
        {sf::Keyboard::Slash, "/"}, {sf::Keyboard::BackSlash, "\\"}, {sf::Keyboard::Tilde, "~"},
        {sf::Keyboard::Equal, "="}, {sf::Keyboard::Dash, "-"}, {sf::Keyboard::Space, "Space"},
        {sf::Keyboard::Return, "Enter"}, {sf::Keyboard::BackSpace, "Backspace"},
        {sf::Keyboard::Tab, "Tab"}, {sf::Keyboard::PageUp, "Page Up"}, {sf::Keyboard::PageDown, "Page Down"},
        {sf::Keyboard::End, "End"}, {sf::Keyboard::Home, "Home"}, {sf::Keyboard::Insert, "Insert"},
        {sf::Keyboard::Delete, "Delete"},

        {sf::Keyboard::Add, "+"}, {sf::Keyboard::Subtract, "-"},
        {sf::Keyboard::Multiply, "*"}, {sf::Keyboard::Divide, "/"},

        {sf::Keyboard::Left, "Left"}, {sf::Keyboard::Right, "Right"},
        {sf::Keyboard::Up, "Up"}, {sf::Keyboard::Down, "Down"},

        {sf::Keyboard::Numpad0, "Numpad 0"}, {sf::Keyboard::Numpad1, "Numpad 1"},
        {sf::Keyboard::Numpad2, "Numpad 2"}, {sf::Keyboard::Numpad3, "Numpad 3"},
        {sf::Keyboard::Numpad4, "Numpad 4"}, {sf::Keyboard::Numpad5, "Numpad 5"},
        {sf::Keyboard::Numpad6, "Numpad 6"}, {sf::Keyboard::Numpad7, "Numpad 7"},
        {sf::Keyboard::Numpad8, "Numpad 8"}, {sf::Keyboard::Numpad9, "Numpad 9"},

        {sf::Keyboard::F1, "F1"}, {sf::Keyboard::F2, "F2"}, {sf::Keyboard::F3, "F3"},
        {sf::Keyboard::F4, "F4"}, {sf::Keyboard::F5, "F5"}, {sf::Keyboard::F6, "F6"},
        {sf::Keyboard::F7, "F7"}, {sf::Keyboard::F8, "F8"}, {sf::Keyboard::F9, "F9"},
        {sf::Keyboard::F10, "F10"}, {sf::Keyboard::F11, "F11"}, {sf::Keyboard::F12, "F12"},
        {sf::Keyboard::F13, "F13"}, {sf::Keyboard::F14, "F14"}, {sf::Keyboard::F15, "F15"},

        {sf::Keyboard::Pause, "Pause"}
    };

    const auto it = keyMap.find(key);
    return (it != keyMap.end()) ? it->second : "Unknown";
}

void Game::init()
{
    sf::VideoMode videoMode(
        _configManager.getValue<int>("/view/size/x", 1920),
        _configManager.getValue<int>("/view/size/y", 1080)
    );
    _gameEngine.initWindow(videoMode, 60, "R-Type");
    _gameEngine.window.setFramerateLimit(60);
    _gameEngine.window.setKeyRepeatEnabled(true);
    loadAssets();

    loadingProgress(30);
    _gameEngine.musicManager.addMusic("level1", "level1", _gameEngine.assetManager);
    _gameEngine.musicManager.addMusic("menu", "menu", _gameEngine.assetManager);
    _gameEngine.musicManager.setVolume(10.0f);

    loadingProgress(40);
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
    _gameEngine.registry.register_component<HitAnimationComponent>();
    _gameEngine.registry.register_component<IndicatorComponent>();

    loadingProgress(50);
    Systems::playerInput(*this);
    Systems::playerMovement(*this);
    Systems::gameView(*this);
    Systems::gameEvent(*this);
    Systems::hitAnimation(*this);

    loadingProgress(60);
    _viewEntity = _gameEngine.registry.spawn_entity();
    _gameEngine.registry.add_component(_viewEntity, ViewComponent{_gameEngine.window.getDefaultView()});

    loadingProgress(70);
    _networkingService.init();
    setHandlers();

    loadingProgress(80);
    _configManager.parse("assets/Data/config.json");

    loadingProgress(100);
    setGameState(GameState::MainMenu);
    Scenes::loadMainMenu(*this);
}

void Game::loadAssets()
{
    uint8_t load = 10;

    std::list<std::pair<std::string, std::string>> fonts = {
        {"arial", "assets/Fonts/Arial.ttf"}
    };

    for (auto &[name, path] : fonts) {
        try {
            _gameEngine.assetManager.loadFont(name, path);
        } catch (const std::runtime_error &e) {
            std::cerr << "Error loading assets: " << e.what() << std::endl;
        }
        load += 2;
        loadingProgress(load);
    }

    std::list <std::pair<std::string, std::string>> textures = {
        {"enemie1", "assets/Enemies/enemie1.png"},
        {"player", "assets/Player/player.png"},
        {"player_projectile", "assets/player_projectile.png"},
        {"player_missile", "assets/player_missile.png"},
        {"background", "assets/background.png"},
        {"logo", "assets/logo.png"},
        {"player_anim", "assets/Player/missile_charging.png"}
    };

    for (auto &[name, path] : textures) {
        try {
            _gameEngine.assetManager.loadTexture(name, path);
        } catch (const std::runtime_error &e) {
            std::cerr << "Error loading assets: " << e.what() << std::endl;
        }
        load += 2;
        loadingProgress(load);
    }

    std::list<std::pair<std::string, std::string>> sounds = {
        {"shooting", "assets/shooting_sound.ogg"},
        {"missile_sound", "assets/missile_sound.ogg"}
    };

    for (auto &[name, path] : sounds) {
        try {
            _gameEngine.assetManager.loadSound(name, path);
        } catch (const std::runtime_error &e) {
            std::cerr << "Error loading assets: " << e.what() << std::endl;
        }
        load += 2;
        loadingProgress(load);
    }

    std::list<std::pair<std::string, std::string>> music = {
        {"level1", "assets/music/level1.ogg"},
        {"menu", "assets/music/menu.ogg"}
    };

    for (auto &[name, path] : music) {
        try {
            _gameEngine.assetManager.loadMusic(name, path);
        } catch (const std::runtime_error &e) {
            std::cerr << "Error loading assets: " << e.what() << std::endl;
        }
        load += 2;
        loadingProgress(load);
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
    _gameEngine.registry.run_system<core::ge::DrawableComponent, HitAnimationComponent>();
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
                    auto &[moveUpKey, moveDownKey, moveLeftKey, moveRightKey, fireKey] = *keyBindingOpt.value();
                    auto &[up, down, left, right, fire, fireReleased] = *inputStateOpt.value();

                    auto set_input_state = [&event, isPressed](auto key, bool &state) {
                        if (event.key.code == key) {
                            state = isPressed;
                        }
                    };

                    set_input_state(moveUpKey, up);
                    set_input_state(moveDownKey, down);
                    set_input_state(moveLeftKey, left);
                    set_input_state(moveRightKey, right);
                    set_input_state(fireKey, fire);
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
            addToScene(entity);
            const auto text = _gameEngine.registry.get_component<core::ge::TextComponent>(entity);

            if (keyToUpdate.value() == "Move Up") {
                _gameEngine.keyBindingsConfig.moveUpKey = key;
                text->text.setString("Move Up: " + keyToString(key));
            } else if (keyToUpdate.value() == "Move Down"){
                _gameEngine.keyBindingsConfig.moveDownKey = key;
                text->text.setString("Move Down: " + keyToString(key));
            } else if (keyToUpdate.value() == "Move Left"){
                _gameEngine.keyBindingsConfig.moveLeftKey = key;
                text->text.setString("Move Left: " + keyToString(key));
            } else if (keyToUpdate.value() == "Move Right"){
                _gameEngine.keyBindingsConfig.moveRightKey = key;
                text->text.setString("Move Right: " + keyToString(key));
            } else if (keyToUpdate.value() == "Shoot") {
                _gameEngine.keyBindingsConfig.fireKey = key;
                text->text.setString("Shoot: " + keyToString(key));
            }
            keyToUpdate.reset();
        }
    }
}

void Game::run() {
    _gameEngine.musicManager.playMusic("menu");
    _networkingService.run();

    // TODO: implement a way to load maps at runtimes dynamically
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
            case RoomMenu:
                Scenes::updateRoomMenu(*this);
                break;
            case Settings:
                Scenes::updateSettingsMenu(*this);
                break;
            case Playing:
                Scenes::updateGame(*this);
                break;
            default:
                break;
        }
        sound();
        render();
    }
    _gameEngine.window.close();
    _networkingService.stop();
}

void Game::loadingProgress(const float progress)
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

    sf::RectangleShape progressBar;
    progressBar.setSize(sf::Vector2f(barSize.x * progress / 100, barSize.y));
    progressBar.setFillColor(sf::Color(0, 255, 0));
    progressBar.setPosition(loadingBar.getPosition());

    _gameEngine.window.draw(loadingBar);
    _gameEngine.window.draw(progressBar);
    _gameEngine.window.draw(loadingText);
    _gameEngine.window.display();
}


