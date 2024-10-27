// #pragma once
//
// #include <SFML/Graphics.hpp>
// #include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"
//
// class Game;
// /**
//  * @class Menus
//  * @brief Manages the initialization of different menus like the main menu and room menu in the game.
//  *
//  * The Menus class provides functions to set up different menus and their UI components, such as buttons and sliders.
//  */
// class Menus {
// public:
//     /**
//      * @brief Constructor for the Menus class.
//      *
//      * @param game Reference to the Game instance to access game-related components and services.
//      */
//     Menus(Game& game);
//
//     /**
//      * @brief Initializes the main menu scene.
//      */
//     void initMainMenu();
//
//     /**
//      * @brief Initializes the room menu scene.
//      */
//     void initRoomMenu();
//
//     /**
//      * @brief Initializes the settings menu scene.
//      */
//     void initSettingsMenu();
//
//     /**
//      * @brief Helper function to convert an SFML keyboard key to a string representation.
//      */
//     static std::string keyToString(sf::Keyboard::Key key);
//
//     void updateSettingsMenu(core::ge::KeyBinding& keyBindingsConfig);
//
// private:
//     Game& _game; ///< Reference to the Game instance to access its components.
// };
//
