#include "Scenes.hpp"
#include <SFML/System/Vector2.hpp>

#include "Game.hpp"
#include "EntityFactory.hpp"
#include "../../../game/RequestType.hpp"
#include "../../../game/Components.hpp"
#include "Utils/ClientComponents.hpp"
#include "../../../game/CollisionMask.hpp"

namespace Scenes {
    void loadMainMenu(Game &game)
    {
        auto &gameEngine = game.getGameEngine();
        const auto &config = game.getConfigManager();

        gameEngine.currentScene = Game::GameState::MainMenu;

        const auto windowSize = sf::Vector2u(
            config.getValue<uint32_t>("/view/size/x", 1920.0f),
            config.getValue<uint32_t>("/view/size/y", 1080.0f)
        );
        const float centerX = static_cast<float>(windowSize.x) / 2.0f;
        const float centerY = static_cast<float>(windowSize.y) / 2.0f;

        const sf::Vector2f buttonSize(200.0f, 50.0f);
        constexpr float buttonSpacing = 20.0f;

        game.addToScene(EntityFactory::createImage(
            game,
            sf::Vector2f(0.0f, 0.0f),
            sf::Vector2f(windowSize),
            "background"
        ));

        game.addToScene(EntityFactory::createImage(
            game,
            sf::Vector2f(centerX - 500.0f, centerY - 400.0f),
            sf::Vector2f(1000.0f, 300.0f),
            "logo"
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(centerX - buttonSize.x / 2, centerY - buttonSize.y - buttonSpacing),
            buttonSize,
            "Solo",
            [&game] {
                game.clearScene();
                loadGame(game);
            }
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(centerX - buttonSize.x / 2, centerY),
            buttonSize,
            "Multiplayer",
            [&game] {
                game.clearScene();
                loadRoomMenu(game);
            }
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(centerX - buttonSize.x / 2, centerY + buttonSize.y + buttonSpacing),
            buttonSize,
            "Settings",
            [&game] {
                game.clearScene();
                loadSettingsMenu(game);
            }
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(centerX - buttonSize.x / 2, centerY + 2 * (buttonSize.y + buttonSpacing)),
            buttonSize,
            "Quit",
            [&game] {
                game.closeWindow();
            }
        ));

        game.addToScene(EntityFactory::createBall(game, sf::Vector2f(100, 100)));

        for (const auto& border : createWorldBorders(gameEngine))
            game.addToScene(border);
    }

    void updateMainMenu(Game &game)
    {
        auto &gameEngine = game.getGameEngine();

        gameEngine.registry.run_system<core::ge::ClickableComponent, core::ge::DrawableComponent, core::ge::TextComponent, core::ge::TransformComponent>();
    }

    void loadRoomMenu(Game &game)
    {
        auto &gameEngine = game.getGameEngine();
        auto &config = game.getConfigManager();

        game.setGameState(Game::GameState::RoomMenu);

        auto windowSize = sf::Vector2u(
            config.getValue<uint32_t>("/view/size/x", 1920.0f),
            config.getValue<uint32_t>("/view/size/y", 1080.0f)
        );
        float centerX = static_cast<float>(windowSize.x) / 2.0f;

        sf::Vector2f buttonSize(200.0f, 50.0f);
        float buttonSpacing = 20.0f;
        float padding = 20.0f;

        sf::Text titleText;
        auto font = gameEngine.assetManager.getFont("arial");
        titleText.setFont(font);
        titleText.setString("Pick a room or create one");
        titleText.setCharacterSize(52);
        titleText.setStyle(sf::Text::Bold);
        titleText.setOutlineThickness(2.0f);
        titleText.setOutlineColor(sf::Color::Black);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(centerX - titleText.getGlobalBounds().width / 2, buttonSize.y - buttonSpacing);

        core::ecs::Entity title = gameEngine.registry.spawn_entity();
        game.addToScene(title);
        gameEngine.registry.add_component(title, core::ge::TextComponent{titleText, font});

        game.addToScene(EntityFactory::createImage(
            game,
            sf::Vector2f(0.0f, 0.0f),
            sf::Vector2f(windowSize),
            "background"
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(buttonSize.y, buttonSize.y - buttonSpacing),
            buttonSize,
            "< Go back",
            [&game] {
                game.clearScene();
                loadMainMenu(game);
            }
        ));

        float availableHeight = static_cast<float>(windowSize.y) - (3 * padding) - titleText.getGlobalBounds().height - 50.0f;
        float availableWidth = static_cast<float>(windowSize.x) - (3 * padding);

        float rectWidth = availableWidth / 2;
        sf::Vector2f rectSize(rectWidth, availableHeight);

        sf::RectangleShape leftRect(rectSize);
        leftRect.setPosition(padding, 2 * padding + titleText.getGlobalBounds().height + 50.0f);
        leftRect.setFillColor(sf::Color::Black);
        leftRect.setOutlineThickness(2.0f);
        leftRect.setOutlineColor(sf::Color::White);

        core::ecs::Entity leftEntity = gameEngine.registry.spawn_entity();
        game.addToScene(leftEntity);
        gameEngine.registry.add_component(leftEntity, core::ge::DrawableComponent{leftRect});

        sf::RectangleShape rightRect(rectSize);
        rightRect.setPosition(2 * padding + rectWidth, 2 * padding + titleText.getGlobalBounds().height + 50.0f);
        rightRect.setFillColor(sf::Color::Black);
        rightRect.setOutlineThickness(2.0f);
        rightRect.setOutlineColor(sf::Color::White);

        core::ecs::Entity rightEntity = gameEngine.registry.spawn_entity();
        game.addToScene(rightEntity);
        gameEngine.registry.add_component(rightEntity, core::ge::DrawableComponent{rightRect});


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

        game.addToScene(EntityFactory::createTextInput(
            game,
            ipInputPosition,
            buttonSize,
            "Enter IP Address"
        ));

        game.addToScene(EntityFactory::createTextInput(
            game,
            portInputPosition,
            buttonSize,
            "Enter Port"
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            createRoomPosition,
            buttonSize,
            "Create Room",
            [] {
                std::cout << "Creating room" << std::endl;
            }
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            startButtonPosition,
            buttonSize,
            "Room 1",
            [&game] {
                game.clearScene();
                loadSettingsMenu(game);
            }
        ));
    }

    void updateRoomMenu(Game &game)
    {
        auto &gameEngine = game.getGameEngine();

        gameEngine.registry.run_system<core::ge::ClickableComponent, core::ge::DrawableComponent, core::ge::TextComponent, core::ge::TransformComponent>();
    }

    void loadSettingsMenu(Game &game)
    {
        auto &gameEngine = game.getGameEngine();
        auto &config = game.getConfigManager();

        game.setGameState(Game::GameState::Settings);

        auto windowSize = sf::Vector2u(
            config.getValue<uint32_t>("/view/size/x", 1920.0f),
            config.getValue<uint32_t>("/view/size/y", 1080.0f)
        );
        float centerX = static_cast<float>(windowSize.x) / 2.0f;

        sf::Vector2f buttonSize(200.0f, 50.0f);
        float buttonSpacing = 40.0f;
        float labelSpacing = 10.0f;

        sf::Text titleText;
        auto font = gameEngine.assetManager.getFont("arial");
        titleText.setFont(font);
        titleText.setString("Settings");
        titleText.setCharacterSize(52);
        titleText.setStyle(sf::Text::Bold);
        titleText.setOutlineThickness(2.0f);
        titleText.setOutlineColor(sf::Color::Black);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(centerX - titleText.getGlobalBounds().width / 2, buttonSize.y - buttonSpacing);

        core::ecs::Entity title = gameEngine.registry.spawn_entity();
        game.addToScene(title);
        gameEngine.registry.add_component(title, core::ge::TextComponent{titleText, font});

        game.addToScene(EntityFactory::createImage(
            game,
            sf::Vector2f(0.0f, 0.0f),
            sf::Vector2f(windowSize),
            "background"
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(buttonSize.y, buttonSize.y - 20.0f),
            buttonSize,
            "< Go back",
            [&game] {
                game.clearScene();
                loadMainMenu(game);
            }
        ));

        game.addToScene(EntityFactory::createSlider(
            game,
            sf::Vector2f(buttonSize.y, buttonSize.y * 3 + buttonSpacing),
            sf::Vector2f(200.0f, 10.0f),
            "Volume",
            [&gameEngine](const float value) {
                gameEngine.musicManager.setVolume(value);
            },
            gameEngine.musicManager.getVolume()
        ));

        float startY = buttonSize.y * 4 + buttonSpacing * 3;
        const auto& keyBindings = gameEngine.keyBindingsConfig;

        auto createKeyBindingDisplay = [&](const std::string& label, sf::Keyboard::Key key, float y) {
            sf::Text text;
            text.setFont(font);
            text.setString(label + ": " + Game::keyToString(key));
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(buttonSize.y, y);

            const core::ecs::Entity entity = gameEngine.registry.spawn_entity();
            game.addToScene(entity);
            gameEngine.registry.add_component(entity, core::ge::TextComponent{text, font});

            game.keyBindingTexts[label] = entity;
        };

        createKeyBindingDisplay("Move Up", keyBindings.moveUpKey, (startY - buttonSize.y) + labelSpacing);
        createKeyBindingDisplay("Move Down", keyBindings.moveDownKey, (startY + buttonSize.y) + labelSpacing);
        createKeyBindingDisplay("Move Left", keyBindings.moveLeftKey, (startY + 3 * buttonSize.y) + labelSpacing);
        createKeyBindingDisplay("Move Right", keyBindings.moveRightKey, (startY + 5 * buttonSize.y) + labelSpacing);
        createKeyBindingDisplay("Shoot", keyBindings.fireKey, (startY + 7 * buttonSize.y) + labelSpacing);

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(buttonSize.y, startY),
            buttonSize,
            "Move Up",
            [&game] {
                game.keyToUpdate = "Move Up";
            }
        ));
        startY += buttonSpacing + buttonSize.y + labelSpacing;

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(buttonSize.y, startY),
            buttonSize,
            "Move Down",
            [&game]  {
                game.keyToUpdate = "Move Down";
            }
        ));
        startY += buttonSpacing + buttonSize.y + labelSpacing;

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(buttonSize.y, startY),
            buttonSize,
            "Move Left",
            [&game] {
                game.keyToUpdate = "Move Left";
            }
        ));
        startY += buttonSpacing + buttonSize.y + labelSpacing;

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(buttonSize.y, startY),
            buttonSize,
            "Move Right",
            [&game] {
                game.keyToUpdate = "Move Right";
            }
        ));
        startY += buttonSpacing + buttonSize.y + labelSpacing;

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(buttonSize.y, startY),
            buttonSize,
            "Shoot",
            [&game] {
                game.keyToUpdate = "Shoot";
            }
        ));
        startY += buttonSpacing + buttonSize.y * 2 + labelSpacing;

        core::ecs::Entity autoFireButtonEntity = EntityFactory::createButton(
        game,
        sf::Vector2f(buttonSize.y, startY),
        sf::Vector2f(300.0f, 50.0f),
        std::string("Auto-fire Mode: ") + (game._autoFire ? "On" : "Off"),
        nullptr
        );

        auto clickable = game.getGameEngine().registry.get_component<core::ge::ClickableComponent>(autoFireButtonEntity);
        clickable->onClick = [&game, autoFireButtonEntity]() {
            game._autoFire = !game._autoFire;
            auto text = game.getGameEngine().registry.get_component<core::ge::TextComponent>(autoFireButtonEntity);
            text->text.setString(std::string("Auto-fire Mode: ") + (game._autoFire ? "On" : "Off"));
        };

        game.addToScene(autoFireButtonEntity);
    }

    void updateSettingsMenu(Game &game)
    {
        auto &gameEngine = game.getGameEngine();

        gameEngine.registry.run_system<core::ge::ClickableComponent, core::ge::DrawableComponent, core::ge::TextComponent, core::ge::TransformComponent>();
    }

    void loadGame(Game &game)
    {
        auto &gameEngine = game.getGameEngine();
        auto &networkingService = game.getNetworkingService();

        gameEngine.currentScene = Game::GameState::Playing;
        gameEngine.musicManager.playMusic("level1");

        game.addToScene(EntityFactory::createGameEventManager(game));

        game.setPlayerConnectionHeader(networkingService.sendRequest("127.0.0.1", 1111, PlayerConnect, {}));
        networkingService.sendRequest("127.0.0.1", 1111, GameStart, {});
    }

    void updateGame(Game &game)
    {
        auto &gameEngine = game.getGameEngine();

        gameEngine.registry.run_system<EventComponent>();

        gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::VelocityComponent, Player>();
        gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::VelocityComponent, InputStateComponent, ShootCounterComponent, Player, core::ge::AnimationComponent>();
    }

    std::vector<core::ecs::Entity> createWorldBorders(core::GameEngine& engine) {
        const float BORDER_THICKNESS = 1.0f;
        const auto windowSize = engine.window.getSize();
        std::vector<core::ecs::Entity> borders;

        auto topBorder = engine.registry.spawn_entity();
        engine.registry.add_component(topBorder, core::ge::TransformComponent{
            {0, 20}, {static_cast<float>(windowSize.x), BORDER_THICKNESS}, {1, 1}, 0
        });
        engine.registry.add_component(topBorder, core::ge::CollisionComponent{
            WORLD_BORDER, {sf::FloatRect(0, 0, static_cast<float>(windowSize.x), BORDER_THICKNESS)}
        });
        borders.push_back(topBorder);

        auto bottomBorder = engine.registry.spawn_entity();
        engine.registry.add_component(bottomBorder, core::ge::TransformComponent{
            {0, static_cast<float>(windowSize.y) - BORDER_THICKNESS}, {static_cast<float>(windowSize.x), BORDER_THICKNESS}, {1, 1}, 0
        });
        engine.registry.add_component(bottomBorder, core::ge::CollisionComponent{
            WORLD_BORDER, {sf::FloatRect(0, 0, static_cast<float>(windowSize.x), BORDER_THICKNESS)}
        });
        borders.push_back(bottomBorder);

        auto leftBorder = engine.registry.spawn_entity();
        engine.registry.add_component(leftBorder, core::ge::TransformComponent{
            {-BORDER_THICKNESS, 0}, {BORDER_THICKNESS, static_cast<float>(windowSize.y)}, {1, 1}, 0
        });
        engine.registry.add_component(leftBorder, core::ge::CollisionComponent{
            WORLD_BORDER, {sf::FloatRect(0, 0, BORDER_THICKNESS, static_cast<float>(windowSize.y))}
        });
        borders.push_back(leftBorder);

        auto rightBorder = engine.registry.spawn_entity();
        engine.registry.add_component(rightBorder, core::ge::TransformComponent{
            {static_cast<float>(windowSize.x), 0}, {BORDER_THICKNESS, static_cast<float>(windowSize.y)}, {1, 1}, 0
        });
        engine.registry.add_component(rightBorder, core::ge::CollisionComponent{
            WORLD_BORDER, {sf::FloatRect(0, 0, BORDER_THICKNESS, static_cast<float>(windowSize.y))}
        });
        borders.push_back(rightBorder);

        return borders;
    }

};
