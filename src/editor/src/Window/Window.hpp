/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Window
*/

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <SFML/Graphics.hpp>
#include "../EventManager/EventManager.hpp"
#include <unordered_set>
#include <memory>
#include <nlohmann/json.hpp>
#include "../Gui/MainMenuBar/MainMenuBar.hpp"
#include "../Gui/ObjectSelector/ObjectSelector.hpp"
#include "../TileSet/TileSet.hpp"
#include "../Map/Map.hpp"

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
        sf::View _view;
        float _viewSpeed = 5.0f;
        float _zoomLevel;
        sf::Clock _deltaTime;
        sf::Vector2f _viewOffset;
        EventManager _eventManager;
        std::unordered_set<sf::Keyboard::Key> _keysPressed;
        sf::Vector2i _lastMousePosition;
        void updateViewOffset();
        Map _map;
        MainMenuBar _mainMenuBar;
        ObjectSelector _objectSelector;
        std::string _filePath;
        bool _popupLoaderIsOpen = false;
        void updateObjectSelector();

        size_t _currentTileSetIndex;
        std::unique_ptr<Tile> _currentTile;
        std::vector<std::unique_ptr<TileSet>> _tileSets;
        void loadTileSetDialog();
        void loadTileSet(const std::string& filePath, int tileWidth, int tileHeight);
        const Tile& getTile(int tileId) const;
        void drawLayers();

        void handleMouseButtonPressed(const sf::Event& event);
        void handleMouseButtonReleased(const sf::Event& event);
        void handleMouseMoved(const sf::Event& event);

        static std::string openSaveFileDialog();

        void setupMainMenuBar();
        static void newMap();
        static void openMap();
        void saveMap();
        static void undo();
        static void redo();
        static void resetView();
    };
}

#endif /* !WINDOW_HPP_ */
