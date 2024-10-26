#include "Menus.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <unordered_map>
#include "EntityFactory.hpp"
#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../core/network/NetworkService.hpp"
#include "../../../game/RequestType.hpp"
#include "../../../game/Components.hpp"

Menus::Menus(Game& game) : _game(game) {
}

std::string Menus::keyToString(sf::Keyboard::Key key)
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

    auto it = keyMap.find(key);
    return (it != keyMap.end()) ? it->second : "Unknown";
}

void Menus::initMainMenu()
{
    sf::Vector2u windowSize = _game._gameEngine.window.getSize();
    float centerX = static_cast<float>(windowSize.x) / 2.0f;
    float centerY = static_cast<float>(windowSize.y) / 2.0f;

    sf::Vector2f buttonSize(200.0f, 50.0f);
    float buttonSpacing = 20.0f;

    EntityFactory::createImage(
        _game._gameEngine,
        sf::Vector2f(0.0f, 0.0f),
        sf::Vector2f(static_cast<float>(_game._gameEngine.window.getSize().x), static_cast<float>(windowSize.y)),
        "background",
        static_cast<int>(Game::GameState::MainMenu)
    );

    EntityFactory::createImage(
        _game._gameEngine,
        sf::Vector2f(centerX - 500.0f, centerY - 400.0f),
        sf::Vector2f(1000.0f, 300.0f),
        "logo",
        static_cast<int>(Game::GameState::MainMenu)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(centerX - buttonSize.x / 2, centerY - buttonSize.y - buttonSpacing),
        buttonSize,
        "Solo",
        [this]() {
            _game._gameEngine.currentScene = static_cast<int>(Game::GameState::Playing);
            _game.playerConnectionHeader = _game.networkingService.sendRequest("127.0.0.1", 1111, PlayerConnect, {});
            _game.networkingService.sendRequest("127.0.0.1", 1111, GameStart, {});
        },
        static_cast<int>(Game::GameState::MainMenu)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(centerX - buttonSize.x / 2, centerY),
        buttonSize,
        "Multiplayer",
        [this]() {
            _game._gameEngine.currentScene = static_cast<int>(Game::GameState::RoomMenu);
        },
        static_cast<int>(Game::GameState::MainMenu)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(centerX - buttonSize.x / 2, centerY + buttonSize.y + buttonSpacing),
        buttonSize,
        "Settings",
        [this]() {
            _game._gameEngine.currentScene = static_cast<int>(Game::GameState::Settings);
        },
        static_cast<int>(Game::GameState::MainMenu)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(centerX - buttonSize.x / 2, centerY + 2 * (buttonSize.y + buttonSpacing)),
        buttonSize,
        "Quit",
        [this]() {
            _game._windowOpen = false;
            _game.networkingService.stop();
        },
        static_cast<int>(Game::GameState::MainMenu)
    );
}

void Menus::initRoomMenu()
{
    sf::Vector2u windowSize = _game._gameEngine.window.getSize();
    float centerX = static_cast<float>(windowSize.x) / 2.0f;

    sf::Vector2f buttonSize(200.0f, 50.0f);
    float buttonSpacing = 20.0f;
    float padding = 20.0f;

    sf::Text titleText;
    auto font = _game._gameEngine.assetManager.getFont("arial");
    titleText.setFont(font);
    titleText.setString("Pick a room or create one");
    titleText.setCharacterSize(52);
    titleText.setStyle(sf::Text::Bold);
    titleText.setOutlineThickness(2.0f);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(centerX - titleText.getGlobalBounds().width / 2, buttonSize.y - buttonSpacing);

    core::ecs::Entity title = _game._gameEngine.registry.spawn_entity();
    _game._gameEngine.registry.add_component(title, core::ge::TextComponent{titleText, font});
    _game._gameEngine.registry.add_component(title, core::ge::SceneComponent{static_cast<int>(Game::GameState::RoomMenu)});

    EntityFactory::createImage(
        _game._gameEngine,
        sf::Vector2f(0.0f, 0.0f),
        sf::Vector2f(static_cast<float>(_game._gameEngine.window.getSize().x), static_cast<float>(windowSize.y)),
        "background",
        static_cast<int>(Game::GameState::RoomMenu)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, buttonSize.y - buttonSpacing),
        buttonSize,
        "< Go back",
        [this]() {
            _game._gameEngine.currentScene = static_cast<int>(Game::GameState::MainMenu);
        },
        static_cast<int>(Game::GameState::RoomMenu)
    );

    float availableHeight = static_cast<float>(windowSize.y) - (3 * padding) - titleText.getGlobalBounds().height - 50.0f;
    float availableWidth = static_cast<float>(windowSize.x) - (3 * padding);

    float rectWidth = availableWidth / 2;
    sf::Vector2f rectSize(rectWidth, availableHeight);

    sf::RectangleShape leftRect(rectSize);
    leftRect.setPosition(padding, 2 * padding + titleText.getGlobalBounds().height + 50.0f);
    leftRect.setFillColor(sf::Color::Black);
    leftRect.setOutlineThickness(2.0f);
    leftRect.setOutlineColor(sf::Color::White);

    core::ecs::Entity leftEntity = _game._gameEngine.registry.spawn_entity();
    _game._gameEngine.registry.add_component(leftEntity, core::ge::DrawableComponent{leftRect});
    _game._gameEngine.registry.add_component(leftEntity, core::ge::SceneComponent{static_cast<int>(Game::GameState::RoomMenu)});

    sf::RectangleShape rightRect(rectSize);
    rightRect.setPosition(2 * padding + rectWidth, 2 * padding + titleText.getGlobalBounds().height + 50.0f);
    rightRect.setFillColor(sf::Color::Black);
    rightRect.setOutlineThickness(2.0f);
    rightRect.setOutlineColor(sf::Color::White);

    core::ecs::Entity rightEntity = _game._gameEngine.registry.spawn_entity();
    _game._gameEngine.registry.add_component(rightEntity, core::ge::DrawableComponent{rightRect});
    _game._gameEngine.registry.add_component(rightEntity, core::ge::SceneComponent{static_cast<int>(Game::GameState::RoomMenu)});


    sf::Vector2f startButtonPosition(
        rightRect.getPosition().x + padding,
        rightRect.getPosition().y + padding
    );

    sf::Vector2f ipInputPosition(
        leftRect.getPosition().x + buttonSize.x / 2 + padding,
        leftRect.getPosition().y + padding * 3
    );

    sf::Vector2f portInputPosition(
        leftRect.getPosition().x + buttonSize.x * 2 + buttonSize.x / 2 + padding * 2,
        leftRect.getPosition().y + padding * 3
    );

    sf::Vector2f createRoomPosition(
        (ipInputPosition.x + portInputPosition.x) / 2,
        ipInputPosition.y + buttonSize.y  * 2 + padding
    );

    EntityFactory::createTextInput(
        _game._gameEngine,
        ipInputPosition,
        buttonSize,
        "Enter IP Address",
        static_cast<int>(Game::GameState::RoomMenu)
    );

    EntityFactory::createTextInput(
        _game._gameEngine,
        portInputPosition,
        buttonSize,
        "Enter Port",
        static_cast<int>(Game::GameState::RoomMenu)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        createRoomPosition,
        buttonSize,
        "Create Room",
        [this]() {
            std::cout << "Creating room" << std::endl;
        },
        static_cast<int>(Game::GameState::RoomMenu)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        startButtonPosition,
        buttonSize,
        "Room 1",
        [this]() {
            _game._gameEngine.currentScene = static_cast<int>(Game::GameState::Playing);
            _game.playerConnectionHeader = _game.networkingService.sendRequest("127.0.0.1", 1111, PlayerConnect, {});
            _game.networkingService.sendRequest("127.0.0.1", 1111, GameStart, {});
        },
        static_cast<int>(Game::GameState::RoomMenu)
    );
}

void Menus::initSettingsMenu()
{
    sf::Vector2u windowSize = _game._gameEngine.window.getSize();
    float centerX = static_cast<float>(windowSize.x) / 2.0f;
    //float centerY = static_cast<float>(windowSize.y) / 2.0f;

    sf::Vector2f buttonSize(200.0f, 50.0f);
    float buttonSpacing = 40.0f;
    float labelSpacing = 10.0f;

    sf::Text titleText;
    auto font = _game._gameEngine.assetManager.getFont("arial");
    titleText.setFont(font);
    titleText.setString("Settings");
    titleText.setCharacterSize(52);
    titleText.setStyle(sf::Text::Bold);
    titleText.setOutlineThickness(2.0f);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(centerX - titleText.getGlobalBounds().width / 2, buttonSize.y - buttonSpacing);

    core::ecs::Entity title = _game._gameEngine.registry.spawn_entity();
    _game._gameEngine.registry.add_component(title, core::ge::TextComponent{titleText, font});
    _game._gameEngine.registry.add_component(title, core::ge::SceneComponent{static_cast<int>(Game::GameState::Settings)});

    EntityFactory::createImage(
        _game._gameEngine,
        sf::Vector2f(0.0f, 0.0f),
        sf::Vector2f(static_cast<float>(_game._gameEngine.window.getSize().x), static_cast<float>(windowSize.y)),
        "background",
        static_cast<int>(Game::GameState::Settings)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, buttonSize.y - 20.0f),
        buttonSize,
        "< Go back",
        [this]() {
            _game._gameEngine.currentScene = static_cast<int>(Game::GameState::MainMenu);
        },
        static_cast<int>(Game::GameState::Settings)
    );

    EntityFactory::createSlider(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, buttonSize.y * 3 + buttonSpacing),
        sf::Vector2f(200.0f, 10.0f),
        "Volume",
        [this](float value) { _game._gameEngine.musicManager.setVolume(value); },
        static_cast<int>(Game::GameState::Settings),
        _game._gameEngine.musicManager.getVolume()
    );

    float startY = buttonSize.y * 4 + buttonSpacing * 3;
    const auto& keyBindings = _game._gameEngine.keyBindingsConfig;

    auto createKeyBindingDisplay = [&](const std::string& label, sf::Keyboard::Key key, float y) {
        sf::Text text;
        text.setFont(font);
        text.setString(label + ": " + keyToString(key));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(buttonSize.y, y);
        _game.keyBindingTexts[label] = text;

        core::ecs::Entity entity = _game._gameEngine.registry.spawn_entity();
        _game._gameEngine.registry.add_component(entity, core::ge::TextComponent{text, font});
        _game._gameEngine.registry.add_component(entity, core::ge::SceneComponent{static_cast<int>(Game::GameState::Settings)});

        y += text.getGlobalBounds().height + labelSpacing;
    };

    createKeyBindingDisplay("Move Up", keyBindings.moveUpKey, (startY - buttonSize.y) + labelSpacing);
    createKeyBindingDisplay("Move Down", keyBindings.moveDownKey, (startY + buttonSize.y) + labelSpacing);
    createKeyBindingDisplay("Move Left", keyBindings.moveLeftKey, (startY + 3 * buttonSize.y) + labelSpacing);
    createKeyBindingDisplay("Move Right", keyBindings.moveRightKey, (startY + 5 * buttonSize.y) + labelSpacing);
    createKeyBindingDisplay("Shoot", keyBindings.fireKey, (startY + 7 * buttonSize.y) + labelSpacing);

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, startY),
        buttonSize,
        "Move Up",
        [this]() {
            _game.keyToUpdate = "Move Up";
        },
        static_cast<int>(Game::GameState::Settings)
    );
    startY += buttonSpacing + buttonSize.y + labelSpacing;

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, startY),
        buttonSize,
        "Move Down",
        [this]() {
            _game.keyToUpdate = "Move Down";
        },
        static_cast<int>(Game::GameState::Settings)
    );
    startY += buttonSpacing + buttonSize.y + labelSpacing;

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, startY),
        buttonSize,
        "Move Left",
        [this]() {
            _game.keyToUpdate = "Move Left";
        },
        static_cast<int>(Game::GameState::Settings)
    );
    startY += buttonSpacing + buttonSize.y + labelSpacing;

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, startY),
        buttonSize,
        "Move Right",
        [this]() {
            _game.keyToUpdate = "Move Right";
        },
        static_cast<int>(Game::GameState::Settings)
    );
    startY += buttonSpacing + buttonSize.y + labelSpacing;

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, startY),
        buttonSize,
        "Shoot",
        [this]() {
            _game.keyToUpdate = "Shoot";
        },
        static_cast<int>(Game::GameState::Settings)
    );

}


