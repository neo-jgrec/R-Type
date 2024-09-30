/*
** EPITECH PROJECT, 2024
** editor
** File description:
** ObjectSelector
*/

#ifndef OBJECTSELECTOR_HPP_
#define OBJECTSELECTOR_HPP_

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include "src/TileSet/TileSet.hpp"

namespace Editor {
    class ObjectSelector {
        public:
            ObjectSelector();
            void render(const std::vector<std::unique_ptr<TileSet>>& tileSets);
            void addObject(const std::string& name);

            [[nodiscard]]
            std::string getSelectedObject() const;

            [[nodiscard]]
            int getSelectedTileSetIndex() const;

            [[nodiscard]]
            bool isObjectSelected() const;
            void setOnObjectSelected(std::function<void(const std::string&)> callback);
            void setSelectedTileSetIndex(int index);
            void clearObjects();
            void updateObjectSelector(const std::vector<std::unique_ptr<TileSet>>& tileSets);
            static void renderTileSetInformation(const std::unique_ptr<TileSet>& tileSet);

        private:
            std::vector<std::string> _objects;
            int _selectedIndex;
            int _selectedTileSetIndex;
            std::function<void(const std::string&)> _onObjectSelected;
            sf::RenderTexture _renderTexture;
            static constexpr float TILE_DISPLAY_SIZE = 50.0f;  // Size of the tile preview
    };
}

#endif /* !OBJECTSELECTOR_HPP_ */