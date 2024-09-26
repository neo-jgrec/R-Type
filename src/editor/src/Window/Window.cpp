/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Window
*/

#include "Window.hpp"
#include <iostream>
#include "../Exceptions.hpp"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <nlohmann/json.hpp>

using namespace Editor;

Window::Window(
    const std::string &title,
    const std::string &mapPath
) : _zoomLevel(1.0f),
    _viewOffset(0.0f, 0.0f),
    _filePath(mapPath)
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    _window.create(desktop, title, sf::Style::Close);
    _view = _window.getDefaultView();
    registerEvents();
    _map.loadMapConfig(mapPath);

    if (!ImGui::SFML::Init(_window))
        throw Editor::Exception("Failed to initialize ImGui-SFML");

    setupMainMenuBar();
    _currentTileSetIndex = -1;
}

Window::~Window()
{
    ImGui::SFML::Shutdown();
}

void Window::loadTileSet(const std::string& filePath, int tileWidth, int tileHeight) {
    std::cout << "Loading TileSet: " << filePath << std::endl;
    _map.loadTileSet(filePath, tileWidth, tileHeight);
    _objectSelector.setSelectedTileSetIndex(static_cast<int>(_map.getTileSets().size()) - 1);
    updateObjectSelector();
}

void Window::updateObjectSelector() {
    _objectSelector.updateObjectSelector(_map.getTileSets());
    _objectSelector.setOnObjectSelected([this](const std::string& objectName) {
        std::cout << "Object selected: " << objectName << std::endl;
        if (objectName == "Erase") {
            _currentTileSetIndex = -1;
            _currentTile = nullptr;
        } else {
            int tileId = std::stoi(objectName);
            try {
                const auto& tile = _map.getTileById(tileId);
                _currentTile = std::make_unique<Tile>(tile);
                _currentTileSetIndex = _map.getTileSets().size() - 1;
            } catch (const Editor::Exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    });
}

void Window::registerEvents() {
    _eventManager.registerEvent(sf::Event::Closed, [this](const sf::Event&) {
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

    _eventManager.registerEvent(sf::Event::MouseButtonPressed, [this](const sf::Event& event) {
        handleMouseButtonPressed(event);
    });

    _eventManager.registerEvent(sf::Event::MouseButtonReleased, [this](const sf::Event& event) {
        handleMouseButtonReleased(event);
    });

    _eventManager.registerEvent(sf::Event::MouseMoved, [this](const sf::Event& event) {
        handleMouseMoved(event);
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
}

void Window::run() {
    while (_window.isOpen()) {
        sf::Event event;
        while (_window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            _eventManager.handleEvent(event);
        }

        updateViewOffset();

        ImGuiIO& io = ImGui::GetIO();
        io.FontGlobalScale = 2.0;

        ImGui::SFML::Update(_window, _deltaTime.restart());

        _mainMenuBar.render();
        _objectSelector.render(_map.getTileSets());
        loadTileSetDialog();

        if (_popupLoaderIsOpen)
            ImGui::OpenPopup("Load TileSet");

        _window.clear();
        _window.setView(_view);
        _map.draw(_window);

        ImGui::SFML::Render(_window);
        _window.display();
    }
}

void Window::setupMainMenuBar() {
    _mainMenuBar.onNewMap = []() { newMap(); };
    _mainMenuBar.onOpenMap = []() { openMap(); };
    _mainMenuBar.onSaveMap = [this]() { saveMap(); };
    _mainMenuBar.onExit = [this]() { _window.close(); };
    _mainMenuBar.onUndo = []() { undo(); };
    _mainMenuBar.onRedo = []() { redo(); };
    _mainMenuBar.onResetView = []() { resetView(); };
    _mainMenuBar.onLoadTileSet = [this]() {
        _popupLoaderIsOpen = true;
    };
}

void Window::loadTileSetDialog() {
    if (ImGui::BeginPopupModal("Load TileSet", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char path[256] = "oak_woods_tileset.png";
        static int tileWidth = 24;
        static int tileHeight = 24;

        ImGui::InputText("File Path", path, IM_ARRAYSIZE(path));
        ImGui::InputInt("Tile Width", &tileWidth);
        ImGui::InputInt("Tile Height", &tileHeight);

        if (ImGui::Button("Load")) {
            loadTileSet(path, tileWidth, tileHeight);
            _popupLoaderIsOpen = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            _popupLoaderIsOpen = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void Window::newMap() {
    std::cout << "New map" << std::endl;
    // TODO: Implement new map functionality
}

void Window::openMap() {
    std::cout << "Open map" << std::endl;
    // TODO: Implement open map functionality
}

void Window::saveMap() {
    std::string fileName = openSaveFileDialog();
    if (!fileName.empty()) {
        _map.saveMap(fileName);
        std::cout << "Map saved successfully to: " << fileName << std::endl;
    } else {
        std::cout << "Save operation cancelled" << std::endl;
    }
}

std::string Window::openSaveFileDialog() {
    return "Untitled.json"; // TODO: Implement file dialog
}

void Window::undo() {
    std::cout << "Undo" << std::endl;
    // TODO: Implement undo functionality
}

void Window::redo() {
    std::cout << "Redo" << std::endl;
    // TODO: Implement redo functionality
}

void Window::resetView() {
    std::cout << "Reset view" << std::endl;
    // TODO: Implement reset view functionality
}

void Window::handleMouseButtonPressed(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        std::cout << "Mouse button pressed" << std::endl;
        sf::Vector2f worldPos = _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), _view);
        int gridX = static_cast<int>(worldPos.x / static_cast<float>(_map.getGrid().getCellSize()));
        int gridY = static_cast<int>(worldPos.y / static_cast<float>(_map.getGrid().getCellSize()));

        std::cout << "Grid X: " << gridX << std::endl;
        std::cout << "Grid Y: " << gridY << std::endl;

        if (_currentTile) {
            std::cout << "Placing tile" << std::endl;
            _map.placeTile(gridX, gridY, _currentTile->getId());
        } else {
            std::cout << "Removing tile" << std::endl;
            _map.removeTile(gridX, gridY);
        }
    }
}

void Window::handleMouseButtonReleased(const sf::Event& event) {
    // No need for implementation in tile-based system
}

void Window::handleMouseMoved(const sf::Event& event) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f worldPos = _window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), _view);
        int gridX = static_cast<int>(worldPos.x / static_cast<float>(_map.getGrid().getCellSize()));
        int gridY = static_cast<int>(worldPos.y / static_cast<float>(_map.getGrid().getCellSize()));

        if (_currentTile) {
            _map.placeTile(gridX, gridY, _currentTile->getId());
        } else {
            _map.removeTile(gridX, gridY);
        }
    }
}

const Tile& Window::getTile(int tileId) const {
    for (const auto& tileSet : _map.getTileSets()) {
        for (int i = 0; i < static_cast<int>(tileSet->getTileCount()); ++i) {
            const Tile& tile = tileSet->getTile(i);
            if (tile.getId() == tileId) {
                return tile;
            }
        }
    }
    throw Editor::Exception("Tile not found");
}