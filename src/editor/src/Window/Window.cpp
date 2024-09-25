/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Window
*/

#include "Window.hpp"
#include <iostream>
#include "../Exceptions.hpp"
#include <fstream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <nlohmann/json.hpp>
using namespace Editor;

Window::Window(
    const std::string &title,
    const std::string &mapPath
) : _grid(1000, 1000, 24),
    _zoomLevel(1.0f),
    _viewOffset(0.0f, 0.0f),
    _filePath(mapPath)
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    _window.create(desktop, title, sf::Style::Close);
    _view = _window.getDefaultView();
    registerEvents();
    loadMapConfig(mapPath);

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
    if (!_tileSets.empty()) {
        const auto& previousTileSet = _tileSets[_tileSets.size() - 1];
        if (previousTileSet->getName() == filePath) {
            std::cout << "TileSet already loaded" << std::endl;
            return;
        }
    }

    _tileSets.push_back(std::make_unique<TileSet>(filePath, tileWidth, tileHeight, filePath));
    _objectSelector.setSelectedTileSetIndex(_tileSets.size() - 1);
    updateObjectSelector();
}

void Window::updateObjectSelector() {
    _objectSelector.updateObjectSelector(_tileSets);
    _objectSelector.setOnObjectSelected([this](const std::string& objectName) {
        std::cout << "Object selected: " << objectName << std::endl;
        if (objectName == "Erase") {
            _currentTileSetIndex = -1;
            _currentTile = nullptr;
        } else {
            int tileId = std::stoi(objectName);
            for (size_t i = 0; i < _tileSets.size(); ++i) {
                for (size_t j = 0; j < _tileSets[i]->getTileCount(); ++j) {
                    const auto& tile = _tileSets[i]->getTile(j);
                    if (tile.getId() == tileId) {
                        _currentTile = std::make_unique<Tile>(tile);
                        _currentTileSetIndex = i;
                        return;
                    }
                }
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
    _grid.updateGrid(_zoomLevel);
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
        _objectSelector.render(_tileSets);
        loadTileSetDialog();

        if (_popupLoaderIsOpen)
            ImGui::OpenPopup("Load TileSet");

        _window.clear();
        _window.setView(_view);
        _grid.draw(_window);

        if (!_tileSets.empty()) {
            for (int y = 0; y < _grid.getGridHeight(); ++y) {
                for (int x = 0; x < _grid.getGridWidth(); ++x) {
                    const auto& tileId = _tileMap[y][x];
                    if (tileId != -1) {
                        const auto& tile = _tileSets[_currentTileSetIndex]->getTile(tileId);
                        tile.draw(_window, x * _grid.getCellSize(), y * _grid.getCellSize());
                    }
                }
            }
        }

        ImGui::SFML::Render(_window);
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

    _tileMap.resize(mapHeight, std::vector<int>(mapWidth, -1));

    if (data.contains("tiles")) {
        for (const auto& tile : data["tiles"]) {
            int x = tile["x"];
            int y = tile["y"];
            int tileIndex = tile["tileIndex"];
            _tileMap[y][x] = tileIndex;
        }
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
    json mapData;
    mapData["width"] = _grid.getGridWidth();
    mapData["height"] = _grid.getGridHeight();

    json tilesArray = json::array();
    for (int y = 0; y < _grid.getGridHeight(); ++y) {
        for (int x = 0; x < _grid.getGridWidth(); ++x) {
            int tileIndex = _tileMap[y][x];
            if (tileIndex != -1) {
                json tile;
                tile["x"] = x;
                tile["y"] = y;
                tile["tileIndex"] = tileIndex;
                tilesArray.push_back(tile);
            }
        }
    }
    mapData["tiles"] = tilesArray;

    std::string fileName = openSaveFileDialog();
    if (!fileName.empty()) {
        std::ofstream file(fileName);
        if (file.is_open()) {
            file << std::setw(4) << mapData << std::endl;
            file.close();
            std::cout << "Map saved successfully to: " << fileName << std::endl;
        } else {
            std::cerr << "Failed to open file for writing: " << fileName << std::endl;
        }
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

void Window::placeTile(int x, int y, int tileIndex) {
    if (x >= 0 && x < _grid.getGridWidth() && y >= 0 && y < _grid.getGridHeight()) {
        _tileMap[y][x] = tileIndex;
    }
}

void Window::removeTile(int x, int y) {
    if (x >= 0 && x < _grid.getGridWidth() && y >= 0 && y < _grid.getGridHeight()) {
        _tileMap[y][x] = -1;
    }
}

void Window::handleMouseButtonPressed(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f worldPos = _window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), _view);
        int gridX = static_cast<int>(worldPos.x / _grid.getCellSize());
        int gridY = static_cast<int>(worldPos.y / _grid.getCellSize());

        if (_currentTile) {
            placeTile(gridX, gridY, _currentTile->getId());
        } else {
            removeTile(gridX, gridY);
        }
    }
}

void Window::handleMouseButtonReleased(const sf::Event& event) {
    // No need for implementation in tile-based system
}

void Window::handleMouseMoved(const sf::Event& event) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f worldPos = _window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), _view);
        int gridX = static_cast<int>(worldPos.x / _grid.getCellSize());
        int gridY = static_cast<int>(worldPos.y / _grid.getCellSize());

        if (_currentTile) {
            placeTile(gridX, gridY, _currentTile->getId());
        } else {
            removeTile(gridX, gridY);
        }
    }
}
