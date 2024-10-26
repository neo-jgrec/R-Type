#pragma once

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json_fwd.hpp>
#include "../../../core/ecs/Entity/Entity.hpp"
#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../core/network/NetworkService.hpp"
#include "../../../core/config/ConfigManager.hpp"
#include "Menus.hpp"

/**
 * @struct Tile
 * @brief Defines the properties of a tile in the game world, including its entity, position, and whether it is destructible.
 */
struct Tile {
    core::ecs::Entity entity;   ///< The entity associated with this tile.
    sf::Vector2f position;      ///< The position of the tile in the game world.
    bool isDestructible;        ///< Indicates if the tile can be destroyed.
};

/**
 * @class Game
 * @brief Manages the game logic, including initialization, event handling, rendering, and updating game state.
 * 
 * The Game class coordinates different game systems and runs the game loop, which processes events,
 * updates game components, and renders the current state. It also manages key entities such as players, enemies,
 * and game states like the main menu and gameplay.
 */
class Game {
public:
    /**
     * @brief Constructs a Game object and initializes the game.
     *
     * The constructor initializes the game by calling `init()`, which sets up entities, components, and systems.
     */
    Game() {
        init();
    }

    /**
     * @brief Destructor for the Game class.
     *
     * The destructor does not perform any specific cleanup operations. Defaulted.
     */
    ~Game() = default;

    /**
     * @brief Runs the game loop, which processes events, updates the game state, and renders the scene.
     *
     * The game loop continues running until the game window is closed.
     */
    void run();

    /**
     * @brief Releases a previously assigned color back into the available pool.
     * @param color The color identifier to be released.
     */
    void releaseColor(int color);

    /**
     * @enum GameState
     * @brief Represents the various states of the game.
     */
    enum class GameState {
        MainMenu = 0,
        RoomMenu = 1,
        Settings = 2,
        Playing = 3,
        GameOver = 4
    };

    Menus menus = Menus(*this); ///< Manages the initialization of different menus in the game.
    core::GameEngine _gameEngine; ///< Game engine responsible for managing entities, components, and systems.
    NetworkingService &networkingService = NetworkingService::getInstance(); ///< Singleton instance of the networking service.
    ConfigManager _configManager;
    GDTPHeader playerConnectionHeader{}; ///< Header for player connection requests.
    bool _windowOpen = true;           ///< Flag to track if the game window is open.
    sf::Vector2f gameScale = {1.0f, 1.0f}; ///< Scaling factor for the game view, adjusted during window resizing.
    std::optional<std::string> keyToUpdate; ///< The key to update in the settings menu.
    std::map<std::string, sf::Text> keyBindingTexts; ///< The key bindings for the game.

private:
    /**
     * @brief Processes SFML events like keyboard inputs, window resizing, and window closing.
     *
     * Handles user inputs, key bindings, and dynamically adjusts the game view based on window resizing events.
     */
    void processEvents();

    /**
     * @brief Updates the game systems and runs relevant systems like movement and collision detection.
     */
    void update();

    /**
     * @brief Renders the game by drawing all relevant components on the screen.
     */
    void render();

    /**
     * @brief Initializes the game, setting up components, entities, and music.
     *
     * Registers all necessary components and creates the initial game entities such as players and enemies. Also loads and plays music.
     */
    void init();

    /**
     * @brief Assigns a unique color to a player.
     * @return The color assigned, or -1 if no colors are available.
     */
    int assignColor();

    core::ecs::Entity _viewEntity = core::ecs::Entity();

    GameState _currentState = GameState::MainMenu; ///< The current state of the game.

    /**
     * @brief Handles input system logic for player movement and actions.
     * @param registry The entity-component system registry managing game entities.
     */
    void inputSystem(core::ecs::Registry& registry);

    /**
     * @brief Updates projectile movements based on their velocity.
     * @param registry The entity-component system registry managing game entities.
     */
    void projectileMovementSystem(core::ecs::Registry& registry) const;

    /**
     * @brief Updates enemy movements based on their velocity.
     * @param registry The entity-component system registry managing game entities.
     */
    void enemyMovementSystem(core::ecs::Registry& registry) const;

    std::vector<int> availableColors = {0, 1, 2, 3, 4}; ///< Pool of available colors for players.

    std::vector<std::vector<Tile>> _tileMap; ///< Represents the game map as a grid of tiles.

    /**
     * @brief Parses a JSON map file and creates the corresponding tiles and entities in the game.
     * @param registry The entity-component system registry managing game entities.
     * @param mapFilePath The file path to the JSON map.
     * @param window The SFML render window for the game.
     */
    void parseMap(core::GameEngine& gameEngine, ConfigManager& config, const std::string& mapFilePath, sf::RenderWindow& window);

    /**
     * @brief Initializes the background based on the map data.
     * @param registry The entity-component system registry managing game entities.
     * @param mapData The JSON data representing the map.
     * @param window The SFML render window for the game.
     */
    void initBackground(core::ecs::Registry& registry, nlohmann::json& mapData, sf::RenderWindow& window) const;

    /**
     * @brief Initializes the window based on the configuration.
     */
    void initWindow();

    /**
     * @brief Loads all assets used in the game.
     */
    void loadAssets();

    /**
     * @brief Handles game events such as player collisions, enemy attacks, and power-ups.
     *
     * @param registry
     */
    void eventSystem(core::ecs::Registry& registry);

    /**
     * @brief Updates the view based on the player's position.
     * @param registry The entity-component system registry managing game entities.
     */
    void viewSystem(core::ecs::Registry& registry);
};
