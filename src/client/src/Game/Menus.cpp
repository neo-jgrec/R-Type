#include "Menus.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "EntityFactory.hpp"
#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../core/network/NetworkService.hpp"
#include "../../../game/RequestType.hpp"
#include "../../../game/Components.hpp"

Menus::Menus(Game& game) : _game(game) {
}

std::string Menus::keyToString(sf::Keyboard::Key key)
{
    switch (key) {
        case sf::Keyboard::Up: return "Up";
        case sf::Keyboard::Down: return "Down";
        case sf::Keyboard::Left: return "Left";
        case sf::Keyboard::Right: return "Right";
        case sf::Keyboard::Space: return "Shoot";

        default: return "Unknown";
    }
}

core::ge::KeyBinding Menus::getPlayerKeyBindings()
{
    auto playerEntities = _game._gameEngine.registry.get_entities<Player, core::ge::KeyBinding>();
    return playerEntities.empty() ? core::ge::KeyBinding{} : *_game._gameEngine.registry.get_component<core::ge::KeyBinding>(playerEntities[0]);
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
    float buttonSpacing = 20.0f;

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
        sf::Vector2f(buttonSize.y, buttonSize.y - buttonSpacing),
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

    float startY = buttonSize.y * 3 + buttonSpacing * 2;
    const auto& keyBindings = getPlayerKeyBindings();

    auto createKeyBindingDisplay = [&](const std::string& label, sf::Keyboard::Key key, float y) {
        sf::Text text;
        text.setFont(font);
        text.setString(label + ": " + keyToString(key));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(buttonSize.y * 2 + buttonSpacing, y);
        _game.keyBindingTexts[label] = text;

        core::ecs::Entity entity = _game._gameEngine.registry.spawn_entity();
        _game._gameEngine.registry.add_component(entity, core::ge::TextComponent{text, font});
        _game._gameEngine.registry.add_component(entity, core::ge::SceneComponent{static_cast<int>(Game::GameState::Settings)});
    };

    createKeyBindingDisplay("Move Up", keyBindings.moveUpKey, startY);
    createKeyBindingDisplay("Move Down", keyBindings.moveDownKey, startY + buttonSpacing);
    createKeyBindingDisplay("Move Left", keyBindings.moveLeftKey, startY + buttonSpacing * 2);
    createKeyBindingDisplay("Move Right", keyBindings.moveRightKey, startY + buttonSpacing * 3);
    createKeyBindingDisplay("Shoot", keyBindings.fireKey, startY + buttonSpacing * 4);

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

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, startY + buttonSpacing),
        buttonSize,
        "Move Down",
        [this]() {
            _game.keyToUpdate = "Move Down";
        },
        static_cast<int>(Game::GameState::Settings)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, startY + buttonSpacing * 2),
        buttonSize,
        "Move Left",
        [this]() {
            _game.keyToUpdate = "Move Left";
        },
        static_cast<int>(Game::GameState::Settings)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, startY + buttonSpacing * 3),
        buttonSize,
        "Move Right",
        [this]() {
            _game.keyToUpdate = "Move Right";
        },
        static_cast<int>(Game::GameState::Settings)
    );

    EntityFactory::createButton(
        _game._gameEngine,
        sf::Vector2f(buttonSize.y, startY + buttonSpacing * 4),
        buttonSize,
        "Shoot",
        [this]() {
            _game.keyToUpdate = "Shoot";
        },
        static_cast<int>(Game::GameState::Settings)
    );

}


