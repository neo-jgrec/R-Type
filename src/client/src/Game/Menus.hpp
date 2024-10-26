#pragma once

#include <SFML/Graphics.hpp>

class Game;
/**
 * @class Menus
 * @brief Manages the initialization of different menus like the main menu and room menu in the game.
 * 
 * The Menus class provides functions to set up different menus and their UI components, such as buttons and sliders.
 */
class Menus {
public:
    /**
     * @brief Constructor for the Menus class.
     * 
     * @param game Reference to the Game instance to access game-related components and services.
     */
    Menus(Game& game);

    /**
     * @brief Initializes the main menu scene.
     */
    void initMainMenu();

    /**
     * @brief Initializes the room menu scene.
     */
    void initRoomMenu();

    /**
     * @brief Initializes the settings menu scene.
     */
    void initSettingsMenu();

private:
    Game& _game; ///< Reference to the Game instance to access its components.
};
