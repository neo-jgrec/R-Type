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
#include "../Map/Map.hpp"
#include "src/Gui/MapPropertiesPanel/MapPropertiesPanel.hpp"
#include "src/Gui/TileInfo/TileInfoPanel.hpp"
#include "src/Gui/ToolPanel/ToolPanel.hpp"
#include <stack>
#include <memory>
#include "../Map/Map.hpp"

using json = nlohmann::json;

namespace Editor {
    class Action {
    public:
        virtual void execute(Map& map) = 0;
        virtual void undo(Map& map) = 0;
        virtual ~Action() = default;
    };

    class AddTileAction : public Action {
        int x, y, tileIndex;
    public:
        AddTileAction(int x, int y, int tileIndex) : x(x), y(y), tileIndex(tileIndex) {}
        void execute(Map& map) override {
            map.placeTile(x, y, tileIndex);
        }
        void undo(Map& map) override {
            map.removeTile(x, y);
        }
    };

    class RemoveTileAction : public Action {
        int x, y;
    public:
        RemoveTileAction(int x, int y) : x(x), y(y) {}
        void execute(Map& map) override {
            map.removeTile(x, y);
        }
        void undo(Map& map) override {
            map.placeTile(x, y, 0);
        }
    };

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
        std::vector<std::vector<std::unique_ptr<Tile>>> _currentTiles;
        std::unique_ptr<Tile> _currentTile;
        void loadTileSetDialog();
        void loadTileSet(const std::string& filePath, int tileWidth, int tileHeight);
        const Tile& getTile(int tileId) const;
        void drawLayers();

        void handleMouseButtonPressed(const sf::Event& event);
        void handleMouseButtonReleased(const sf::Event& event);
        void handleMouseMoved(const sf::Event& event);

        std::string openSaveFileDialog();

        void setupMainMenuBar();
        void saveMap();
        void undo();
        void redo();
        void resetView();
        void openMapDialog();
        bool _openMapDialogIsOpen = false;
        void newMapDialog();
        bool _newMapDialogIsOpen = false;
        void aboutDialog();
        bool _aboutDialogIsOpen = false;

        std::vector<sf::Vector2i> _selectedTiles;

        ToolPanel _toolPanel;
        TileInfoPanel _tileInfoPanel;

        void drawPreviewTile();
        void renderUI();

        MapPropertiesPanel _mapPropertiesPanel;

        static constexpr float MIN_ZOOM = 0.1f;
        static constexpr float MAX_ZOOM = 5.0f;

        std::stack<std::unique_ptr<Action>> _undoStack;
        std::stack<std::unique_ptr<Action>> _redoStack;
    };
}

#endif /* !WINDOW_HPP_ */
