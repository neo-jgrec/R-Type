/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Window
*/

#include "Window.hpp"

using namespace Editor;

Window::Window(const std::string &title, const std::string &mapPath)
    : _grid(1000, 1000, 10), _zoomLevel(1.0f), _viewOffset(0.0f, 0.0f)
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    _window.create(desktop, title, sf::Style::Close);
    _view = _window.getDefaultView();
    registerEvents();
    loadMapConfig(mapPath);
}

Window::~Window()
{
}

void Window::registerEvents() {
    _eventManager.registerEvent(sf::Event::Closed, [this](const sf::Event &event) {
        _window.close();
    });

    _eventManager.registerEvent(sf::Event::KeyPressed, [this](const sf::Event &event) {
        if (event.key.code == sf::Keyboard::Escape)
            _window.close();
    });

    _eventManager.registerEvent(sf::Event::MouseWheelScrolled, [this](const sf::Event &event) {
        if (event.mouseWheelScroll.delta > 0) {
            handleZoom(0.9f);
        } else {
            handleZoom(1.1f);
        }
    });

    _eventManager.registerEvent(sf::Event::KeyPressed, [this](const sf::Event &event) {
        _keysPressed.insert(event.key.code);
    });

    _eventManager.registerEvent(sf::Event::KeyReleased, [this](const sf::Event &event) {
        _keysPressed.erase(event.key.code);
    });
}

void Window::updateViewOffset() {
    if (_keysPressed.count(sf::Keyboard::Z))
        _viewOffset.y -= _viewSpeed * _zoomLevel;
    if (_keysPressed.count(sf::Keyboard::S))
        _viewOffset.y += _viewSpeed * _zoomLevel;
    if (_keysPressed.count(sf::Keyboard::Q))
        _viewOffset.x -= _viewSpeed * _zoomLevel;
    if (_keysPressed.count(sf::Keyboard::D))
        _viewOffset.x += _viewSpeed * _zoomLevel;

    _view.setCenter(_viewOffset);
}

void Window::handleZoom(float zoomFactor) {
    _zoomLevel *= zoomFactor;
    _view.zoom(zoomFactor);
    _grid.updateGrid(_zoomLevel);
}

void Window::run() {
    while (_window.isOpen()) {
        sf::Event event;
        while (_window.pollEvent(event))
            _eventManager.handleEvent(event);

        updateViewOffset();

        _window.clear();
        _window.setView(_view);
        _grid.draw(_window);
        for (const auto& spawnPoint : _spawnPoints)
            spawnPoint.draw(_window);
        _window.display();
    }
}

void Window::loadMapConfig(const std::string &mapPath) {
    std::ifstream file(mapPath);
    if (!file.is_open()) {
        throw Editor::Exception("Failed to open map file: " + mapPath);
    }

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& e) {
        throw Editor::Exception("Failed to parse JSON file: " + std::string(e.what()));
    }

    int mapWidth = data["width"];
    int mapHeight = data["height"];
    _grid.setGridSize(mapWidth, mapHeight);
    std::cout << "Map width: " << mapWidth << std::endl;
    std::cout << "Map height: " << mapHeight << std::endl;

    for (const auto& spawnPoint : data["spawnPoints"]) {
        float x = spawnPoint["x"];
        float y = spawnPoint["y"];
        _spawnPoints.emplace_back(x, y);
    }
}
