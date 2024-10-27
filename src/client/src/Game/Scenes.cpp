#include "Scenes.hpp"

#include "Game.hpp"
#include "EntityFactory.hpp"
#include "../../../game/RequestType.hpp"
#include "../../../game/Components.hpp"
#include "Utils/ClientComponents.hpp"

namespace Scenes {
    void loadMainMenu(Game &game)
    {
        auto &gameEngine = game.getGameEngine();
        auto &networkingService = game.getNetworkingService();

        const sf::Vector2u windowSize = gameEngine.window.getSize();
        const float centerX = static_cast<float>(windowSize.x) / 2.0f;
        const float centerY = static_cast<float>(windowSize.y) / 2.0f;

        const sf::Vector2f buttonSize(200.0f, 50.0f);
        float buttonSpacing = 20.0f;

        game.addToScene(EntityFactory::createImage(
            game,
            sf::Vector2f(0.0f, 0.0f),
            sf::Vector2f(static_cast<float>(gameEngine.window.getSize().x), static_cast<float>(windowSize.y)),
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
            "Start Game",
            [&game] {
                game.clearScene();
                loadGame(game);
            }
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(centerX - buttonSize.x / 2, centerY),
            buttonSize,
            "Options",
            []() { }
        ));

        game.addToScene(EntityFactory::createButton(
            game,
            sf::Vector2f(centerX - buttonSize.x / 2, centerY + buttonSize.y + buttonSpacing),
            buttonSize,
            "Quit",
            [&game, &networkingService] {
                game.closeWindow();
                networkingService.stop();
            }
        ));

        game.addToScene(EntityFactory::createSlider(
            game,
            sf::Vector2f(centerX - buttonSize.x / 2, centerY + 3 * (buttonSize.y + (buttonSpacing += 10.0f))),
            sf::Vector2f(200.0f, 10.0f),
            "Volume",
            [&gameEngine](const float value) {
                gameEngine.musicManager.setVolume(value);
            },
            gameEngine.musicManager.getVolume()
        ));
    }

    void updateMainMenu(Game &game) {
        core::GameEngine &gameEngine = game.getGameEngine();

        gameEngine.registry.run_system<core::ge::ClickableComponent, core::ge::DrawableComponent, core::ge::TextComponent, core::ge::TransformComponent>();
    }

    void loadGame(Game &game) {
        core::GameEngine &gameEngine = game.getGameEngine();
        NetworkingService &networkingService = game.getNetworkingService();

        gameEngine.currentScene = Game::GameState::Playing;

        // TODO: implement a way to load maps at runtimes dynamically
        game.addToScene(EntityFactory::createGameEventManager(game));

        game.setPlayerConnectionHeader(networkingService.sendRequest("127.0.0.1", 1111, PlayerConnect, {}));
        networkingService.sendRequest("127.0.0.1", 1111, GameStart, {});
    }

    void updateGame(Game &game) {
        core::GameEngine &gameEngine = game.getGameEngine();

        gameEngine.registry.run_system<EventComponent>();

        gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::VelocityComponent>();
        gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::VelocityComponent, Player>();
        gameEngine.registry.run_system<core::ge::TransformComponent, core::ge::VelocityComponent, InputStateComponent, ShootCounterComponent, Player, core::ge::AnimationComponent>();
    }
};
