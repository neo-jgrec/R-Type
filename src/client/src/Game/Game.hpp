#pragma once

#include <list>
#include <SFML/System/Vector2.hpp>
#include <nlohmann/json_fwd.hpp>
#include "../../../core/ecs/Entity/Entity.hpp"
#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../core/network/NetworkService.hpp"
#include "../../../core/config/ConfigManager.hpp"

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
     * @enum GameState
     * @brief Represents the various states of the game.
     */
    enum GameState: uint8_t {
        MainMenu = 0,
        RoomMenu = 1,
        Settings = 2,
        Playing = 3,
        GameOver = 4,
        Loading = 63
    };

    // Menus menus = Menus(*this); ///< Manages the initialization of different menus in the game.
    core::GameEngine _gameEngine; ///< Game engine responsible for managing entities, components, and systems.
    NetworkingService &networkingService = NetworkingService::getInstance(); ///< Singleton instance of the networking service.
    ConfigManager _configManager;
    GDTPHeader playerConnectionHeader{}; ///< Header for player connection requests.
    bool _windowOpen = true;           ///< Flag to track if the game window is open.
    sf::Vector2f gameScale = {1.0f, 1.0f}; ///< Scaling factor for the game view, adjusted during window resizing.
    std::optional<std::string> keyToUpdate; ///< The key to update in the settings menu.
    std::map<std::string, core::ecs::Entity> keyBindingTexts; ///< The key bindings for the game.
    float _scrollSpeed = 20.0f;
    std::chrono::steady_clock::time_point _lastScrollTime;
    uint32_t _lastScrollPosition = 0;

private:
    NetworkingService &_networkingService = NetworkingService::getInstance(); ///< Singleton instance of the networking service.
    GDTPHeader _playerConnectionHeader{}; ///< Header for player connection requests.
    GameState _gameState = GameState::MainMenu; ///< The current state of the game.
    std::list<core::ecs::Entity> _sceneEntities; ///< List of entities in the current scene.

    /**
     * @brief Processes SFML events like keyboard inputs, window resizing, and window closing.
     *
     * Handles user inputs, key bindings, and dynamically adjusts the game view based on window resizing events.
     */
    void processEvents();

    /**
     * @brief Updates the game systems and runs relevant systems like movement and collision detection.
     */
    void sound();

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
    void inputSystem(Game &game);

    /**
     * @brief Updates player movements based on their velocity.
     * @param registry The entity-component system registry managing game entities.
     */
    void playerMovementSystem(core::ecs::Registry& registry) const;

    std::vector<int> availableColors = {0, 1, 2, 3, 4}; ///< Pool of available colors for players.

    std::vector<std::vector<Tile>> _tileMap; ///< Represents the game map as a grid of tiles.

    float _loadingProgress = 0.0f;

    /**
     * @brief Parses a JSON map file and creates the corresponding tiles and entities in the game.
     * @param registry The entity-component system registry managing game entities.
     * @param mapFilePath The file path to the JSON map.
     * @param window The SFML render window for the game.
     */
    void parseMap(Game &game, const std::string& mapFilePath, sf::RenderWindow& window);

    /**
     * @brief Initializes the background based on the map data.
     * @param registry The entity-component system registry managing game entities.
     * @param mapData The JSON data representing the map.
     * @param window The SFML render window for the game.
     */
    static void initBackground(core::ecs::Registry& registry, nlohmann::json& mapData, sf::RenderWindow& window, sf::Vector2f& gameScale);

    /**
     * @brief Initializes the window based on the configuration.
     */
    void initWindow();

    /**
     * @brief Loads all assets used in the game.
     */
    void loadAssets();

    /**
     * @brief Updates the loading progress of the assets.
     */
    void renderLoadingScreen();

    /**
     * @brief Updates the loading progress of the assets.
     * @param progress The progress of the loading process.
     */
    void updateLoadingProgress(float progress);

    /**
     * @brief Handles game events such as player collisions, enemy attacks, and power-ups.
     *
     * @param registry
     */
    void eventSystem(Game &game);

    /**
     * @brief Updates the view based on the player's position.
     * @param registry The entity-component system registry managing game entities.
     */
    void viewSystem(Game &game);

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

    core::GameEngine &getGameEngine() { return _gameEngine; }
    core::ecs::Registry &getRegistry() { return _gameEngine.registry; }
    NetworkingService &getNetworkingService() { return _networkingService; }
    ConfigManager &getConfigManager() { return _configManager; }
    GameState getGameState() const { return _gameState; }

    void setGameState(const GameState state) { _gameState = state; }
    void setPlayerConnectionHeader(const GDTPHeader &header) { _playerConnectionHeader = header; }

    void closeWindow() { _windowOpen = false; }
    void addToScene(const core::ecs::Entity entity) { _sceneEntities.push_back(entity); }
    void clearScene()
    {
        for (auto entity : _sceneEntities)
            _gameEngine.registry.kill_entity(entity);
        _sceneEntities.clear();
    }

    static std::string keyToString(sf::Keyboard::Key key);
};
