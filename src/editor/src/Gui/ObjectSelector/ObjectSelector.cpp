/*
** EPITECH PROJECT, 2024
** editor
** File description:
** ObjectSelector
*/

#include "ObjectSelector.hpp"
#include <imgui.h>
#include <memory>
#include "src/TileSet/TileSet.hpp"
using namespace Editor;

ObjectSelector::ObjectSelector() : _selectedIndex(-1), _selectedTileSetIndex(-1) {}

void ObjectSelector::render(const std::vector<std::unique_ptr<TileSet>>& tileSets) {
    ImGui::Begin("TileSet");
    float imageSizeMultiplier = 1.5f;

    ImGui::Text("Load a tileset by clicking on the 'Load TileSet' button \rin the file menu.");
    ImGui::Text("Select a tileset from the dropdown menu below.");
    ImGui::Text("Click on a tile to select it.");
    ImGui::Text("The selected tile will be highlighted in red.");
    ImGui::Text("Click on the map to place the selected tile.");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::BeginCombo("TileSet", _selectedTileSetIndex >= 0 ? tileSets[_selectedTileSetIndex]->getName().c_str() : "Select TileSet")) {
        for (int i = 0; i < static_cast<int>(tileSets.size()); ++i) {
            bool isSelected = (i == _selectedTileSetIndex);
            if (ImGui::Selectable(tileSets[i]->getName().c_str(), isSelected)) {
                _selectedTileSetIndex = i;
                updateObjectSelector(tileSets);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (_selectedTileSetIndex < 0 || _selectedTileSetIndex >= static_cast<int>(tileSets.size())) {
        ImGui::End();
        return;
    }

    renderTileSetInformation(tileSets[_selectedTileSetIndex]);

    const auto& selectedTileSet = tileSets[_selectedTileSetIndex];
    sf::Vector2i tileSize = selectedTileSet->getTileSize();
    sf::Vector2f textureSize = selectedTileSet->getTextureSize();
    ImVec2 imagePos = ImGui::GetCursorScreenPos();
    ImGui::Image(selectedTileSet->getTextureID(), ImVec2(textureSize.x * imageSizeMultiplier, textureSize.y * imageSizeMultiplier));
    for (int y = 0; y < static_cast<int>(textureSize.y); y += tileSize.y) {
        for (int x = 0; x < static_cast<int>(textureSize.x); x += tileSize.x) {
            ImGui::GetWindowDrawList()->AddRect(
                ImVec2(imagePos.x + static_cast<float>(x) * imageSizeMultiplier, imagePos.y + static_cast<float>(y) * imageSizeMultiplier),
                ImVec2(imagePos.x + static_cast<float>(x + tileSize.x) * imageSizeMultiplier, imagePos.y + static_cast<float>(y + tileSize.y) * imageSizeMultiplier),
                IM_COL32(255, 255, 255, 100)
            );

            int tileIndex = (y / tileSize.y) * (static_cast<int>(textureSize.x) / tileSize.x) + (x / tileSize.x) + 1;
            int tilesetStartIndex = selectedTileSet->getNextId() - static_cast<int>(selectedTileSet->getTileCount());
            tileIndex += tilesetStartIndex - 1;
            if (std::find(_selectedTileIds.begin(), _selectedTileIds.end(), tileIndex) != _selectedTileIds.end()) {
                ImGui::GetWindowDrawList()->AddRect(
                    ImVec2(imagePos.x + static_cast<float>(x) * imageSizeMultiplier, imagePos.y + static_cast<float>(y) * imageSizeMultiplier),
                    ImVec2(imagePos.x + static_cast<float>(x + tileSize.x) * imageSizeMultiplier, imagePos.y + static_cast<float>(y + tileSize.y) * imageSizeMultiplier),
                    IM_COL32(255, 0, 0, 255),
                    0.5f,
                    0,
                    2.0f
                );
            }
        }
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        ImVec2 mousePos = ImGui::GetMousePos();
        int clickedX = static_cast<int>((mousePos.x - imagePos.x) / imageSizeMultiplier);
        int clickedY = static_cast<int>((mousePos.y - imagePos.y) / imageSizeMultiplier);
        if (clickedX >= 0 && clickedX < static_cast<int>(textureSize.x) && clickedY >= 0 && clickedY < static_cast<int>(textureSize.y)) {
            int tileX = clickedX / tileSize.x;
            int tileY = clickedY / tileSize.y;
            int tileIndex = tileY * (static_cast<int>(textureSize.x) / tileSize.x) + tileX;
            int tilesetStartIndex = selectedTileSet->getNextId() - static_cast<int>(selectedTileSet->getTileCount());
            tileIndex += tilesetStartIndex;
            _selectedIndex = tileIndex;
            auto it = std::find(_selectedTileIds.begin(), _selectedTileIds.end(), tileIndex);
            if (it != _selectedTileIds.end())
                _selectedTileIds.erase(it);
            else
                _selectedTileIds.push_back(tileIndex);
            if (_onObjectSelected)
                _onObjectSelected(std::to_string(tileIndex), tileY);
        }
    }

    ImGui::End();
}

void ObjectSelector::renderTileSetInformation(const std::unique_ptr<TileSet>& tileSet) {
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Name: %s", tileSet->getName().c_str());
    ImGui::Text("Tile Size: %dx%d", (int)tileSet->getTileSize().x, (int)tileSet->getTileSize().y);
    ImGui::Text("Texture Size: %dx%d", (int)tileSet->getTextureSize().x, (int)tileSet->getTextureSize().y);
    ImGui::Text("Tile Count: %d", (int)tileSet->getTileCount());
    ImGui::Separator();
    ImGui::Spacing();
}

void ObjectSelector::addObject(const std::string& name) {
    _objects.push_back(name);
}

std::string ObjectSelector::getSelectedObject() const {
    if (_selectedIndex >= 0 && _selectedIndex < static_cast<int>(_objects.size())) {
        return _objects[_selectedIndex];
    }
    return "";
}

int ObjectSelector::getSelectedTileSetIndex() const {
    return _selectedTileSetIndex;
}

bool ObjectSelector::isObjectSelected() const {
    return _selectedIndex != -1;
}

void ObjectSelector::setOnObjectSelected(std::function<void(const std::string&, int)> callback) {
    _onObjectSelected = std::move(callback);
}

void ObjectSelector::setSelectedTileSetIndex(int index) {
    _selectedTileSetIndex = index;
}

void ObjectSelector::clearObjects() {
    _objects.clear();
}

void ObjectSelector::updateObjectSelector(const std::vector<std::unique_ptr<TileSet>>& tileSets) {
    clearObjects();
    _selectedTileIds.clear();
    addObject("Erase");

    if (_selectedTileSetIndex >= 0 && _selectedTileSetIndex < static_cast<int>(tileSets.size())) {
        const auto& selectedTileSet = tileSets[_selectedTileSetIndex];
        for (int j = 0; j < static_cast<int>(selectedTileSet->getTileCount()); ++j) {
            const auto& tile = selectedTileSet->getTile(j);
            addObject(std::to_string(tile.getId()));
        }
    }
}

void ObjectSelector::clearSelectedTileIds() {
    _selectedTileIds.clear();
}
