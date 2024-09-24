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

namespace Editor {
    class Window {
    public:
        Window(const std::string &title);
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
    };
}

#endif /* !WINDOW_HPP_ */
