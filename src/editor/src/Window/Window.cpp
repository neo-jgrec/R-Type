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
    _filePath(mapPath),
    _currentTileSetIndex(-1)
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    _window.create(desktop, title, sf::Style::Close);
    _view = _window.getDefaultView();
    registerEvents();
    _map.loadMapConfig(mapPath);
    updateObjectSelector();

    if (!ImGui::SFML::Init(_window))
        throw Editor::Exception("Failed to initialize ImGui-SFML");

    setupMainMenuBar();

    _toolPanel.setOnToolSelected([this](Tool tool) {
        switch (tool) {
            case Tool::ERASER:
                _currentTile = nullptr;
                break;
            default:
                break;
        }
    });
}

Window::~Window()
{
    ImGui::SFML::Shutdown();
}

void Window::loadTileSet(const std::string& filePath, int tileWidth, int tileHeight) {
    try {
        _map.loadTileSet(filePath, tileWidth, tileHeight);
        _objectSelector.setSelectedTileSetIndex(static_cast<int>(_map.getTileSets().size()) - 1);
        updateObjectSelector();
        std::cout << "TileSet loaded successfully: " << filePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load TileSet: " << e.what() << std::endl;
    }
}

void Window::updateObjectSelector() {
    std::cout << "Updating object selector" << std::endl;
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
        // Check for Ctrl+S (Save Map)
        if (event.key.control && event.key.code == sf::Keyboard::S)
            saveMap();
        // Check for Ctrl+O (Open Map)
        if (event.key.control && event.key.code == sf::Keyboard::O)
            _openMapDialogIsOpen = true;
        // Check for Ctrl+N (New Map)
        if (event.key.control && event.key.code == sf::Keyboard::N)
            _newMapDialogIsOpen = true;
        // Check for Alt+F4 (Exit)
        if (event.key.alt && event.key.code == sf::Keyboard::F4)
            _window.close();

        if (event.key.control)
            return;
        _keysPressed.insert(event.key.code);
    });

    _eventManager.registerEvent(sf::Event::KeyReleased, [this](const sf::Event &event) {
        _keysPressed.erase(event.key.code);
    });

    _eventManager.registerEvent(sf::Event::MouseWheelScrolled, [this](const sf::Event &event) {
        if (event.mouseWheelScroll.delta > 0) {
            handleZoom(0.9f);
        } else {
            handleZoom(1.1f);
        }
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
    float newZoomLevel = _zoomLevel * zoomFactor;

    if (newZoomLevel >= MIN_ZOOM && newZoomLevel <= MAX_ZOOM) {
        _zoomLevel = newZoomLevel;
        _view.zoom(zoomFactor);
    }
}

void Window::run() {
    sf::Clock deltaClock;
    while (_window.isOpen()) {
        sf::Event event;
        while (_window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            _eventManager.handleEvent(event);
        }

        updateViewOffset();

        ImGuiIO& io = ImGui::GetIO();
        io.FontGlobalScale = 2.0f;

        ImGui::SFML::Update(_window, deltaClock.restart());

        renderUI();

        _window.clear();
        _window.setView(_view);
        _map.draw(_window);

        drawPreviewTile();

        ImGui::SFML::Render(_window);
        _window.display();
    }
}

void Window::renderUI() {
    _mapPropertiesPanel.render(_map);
    _mainMenuBar.render();
    _objectSelector.render(_map.getTileSets());
    _toolPanel.render();
    loadTileSetDialog();
    openMapDialog();
    newMapDialog();
    aboutDialog();

    if (_popupLoaderIsOpen)
        ImGui::OpenPopup("Load TileSet");
    if (_openMapDialogIsOpen)
        ImGui::OpenPopup("Open Map");
    if (_newMapDialogIsOpen)
        ImGui::OpenPopup("New Map");
    if (_aboutDialogIsOpen)
        ImGui::OpenPopup("About");
}

void Window::drawPreviewTile() {
    if (!_currentTile)
        return;
    sf::Vector2i mousePos = sf::Mouse::getPosition(_window);
    sf::Vector2f worldPos = _window.mapPixelToCoords(mousePos, _view);
    int gridX = static_cast<int>(worldPos.x / static_cast<float>(_map.getGrid().getCellSize()));
    int gridY = static_cast<int>(worldPos.y / static_cast<float>(_map.getGrid().getCellSize()));
    _map.drawPreviewTile(gridX, gridY, _currentTile->getId(), _window);
}

void Window::setupMainMenuBar() {
    _mainMenuBar.onNewMap = [this]() {
        _newMapDialogIsOpen = true;
    };
    _mainMenuBar.onOpenMap = [this]() {
        _openMapDialogIsOpen = true;
    };
    _mainMenuBar.onSaveMap = [this]() { saveMap(); };
    _mainMenuBar.onExit = [this]() { _window.close(); };
    _mainMenuBar.onUndo = []() { undo(); };
    _mainMenuBar.onRedo = []() { redo(); };
    _mainMenuBar.onResetView = []() { resetView(); };
    _mainMenuBar.onLoadTileSet = [this]() {
        _popupLoaderIsOpen = true;
    };
    _mainMenuBar.onAbout = [this]() {
        _aboutDialogIsOpen = true;
    };
}

void Window::aboutDialog() {
    if (ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("R-Type Editor");
        ImGui::Text("Version 0.1");
        ImGui::Text("Author: Louis LANGANAY");
        ImGui::Text("Email: louis.langanay@epitech.eu");
        ImGui::Text("Repository: https://github.com/neo-jgrec/R-Type");
        if (ImGui::Button("Close")) {
            _aboutDialogIsOpen = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
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

void Window::newMapDialog() {
    if (ImGui::BeginPopupModal("New Map", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static int mapWidth = 50;
        static int mapHeight = 20;
        static int cellSize = 24;
        static char path[256] = "Untitled.json";

        ImGui::InputInt("Map Width", &mapWidth);
        ImGui::InputInt("Map Height", &mapHeight);
        ImGui::InputInt("Cell Size", &cellSize);
        ImGui::InputText("File Path", path, IM_ARRAYSIZE(path));

        if (ImGui::Button("Create")) {
            try {
                _map.createNewMap(mapWidth, mapHeight, cellSize);
                _filePath = path;
                _zoomLevel = 1.0f;
                _viewOffset = sf::Vector2f(0.0f, 0.0f);
                _view = _window.getDefaultView();
                _currentTileSetIndex = -1;
                _currentTile = nullptr;
                updateObjectSelector();
                _newMapDialogIsOpen = false;
                ImGui::CloseCurrentPopup();
            } catch (const Editor::Exception& e) {
                std::cerr << "Failed to create new map: " << e.what() << std::endl;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            _newMapDialogIsOpen = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void Window::openMapDialog() {
    if (ImGui::BeginPopupModal("Open Map", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char path[256] = "";
        ImGui::InputText("File Path", path, IM_ARRAYSIZE(path));

        if (ImGui::Button("Open")) {
            try {
                _map.clearMap();
                _map.loadMapConfig(path);
                updateObjectSelector();
                _openMapDialogIsOpen = false;
                ImGui::CloseCurrentPopup();
            } catch (const Editor::Exception& e) {
                std::cerr << "Failed to open map: " << e.what() << std::endl;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            _openMapDialogIsOpen = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
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
    if (event.mouseButton.button != sf::Mouse::Left ||
        _openMapDialogIsOpen || _newMapDialogIsOpen || _popupLoaderIsOpen || _aboutDialogIsOpen)
        return;

    sf::Vector2f worldPos = _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), _view);
    int gridX = static_cast<int>(worldPos.x / static_cast<float>(_map.getGrid().getCellSize()));
    int gridY = static_cast<int>(worldPos.y / static_cast<float>(_map.getGrid().getCellSize()));

    if (!_map.isPositionValid(gridX, gridY))
        return;

    if (_currentTile)
        _map.placeTile(gridX, gridY, _currentTile->getId());
    else
        _map.removeTile(gridX, gridY);
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
    throw Editor::Exception("Tile not found: " + std::to_string(tileId));
}