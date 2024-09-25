/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Window
*/

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <SFML/Graphics.hpp>
#include "../Exceptions.hpp"
#include "../Grid/Grid.hpp"
#include "../EventManager/EventManager.hpp"
#include <unordered_set>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include "../SpawnPoint/SpawnPoint.hpp"

using json = nlohmann::json;

namespace Editor {
    class Window {
    public:
        Window(const std::string &title, const std::string &mapPath = "");
        ~Window();
        void run();
        void handleZoom(float zoomFactor);
    private:
        void registerEvents();
        sf::RenderWindow _window;
        Grid _grid;
        sf::View _view;
        float _viewSpeed = 5.0f;
        float _zoomLevel;
        sf::Vector2f _viewOffset;
        EventManager _eventManager;
        std::unordered_set<sf::Keyboard::Key> _keysPressed;
        sf::Vector2i _lastMousePosition;
        void updateViewOffset();
        void loadMapConfig(const std::string &mapPath);
        std::vector<SpawnPoint> _spawnPoints;
    };
}

#endif /* !WINDOW_HPP_ */
