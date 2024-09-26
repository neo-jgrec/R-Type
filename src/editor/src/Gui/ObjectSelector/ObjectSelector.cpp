/*
** EPITECH PROJECT, 2024
** editor
** File description:
** ObjectSelector
*/

#include "ObjectSelector.hpp"
#include <memory>
#include "src/TileSet/TileSet.hpp"
#include <iostream>
using namespace Editor;

ObjectSelector::ObjectSelector() : _selectedIndex(-1), _selectedTileSetIndex(-1) {}

void ObjectSelector::render(const std::vector<std::unique_ptr<TileSet>>& tileSets) {
    ImGui::Begin("Object Selector");

    // TileSet selector
    if (ImGui::BeginCombo("TileSet", _selectedTileSetIndex >= 0 ? tileSets[_selectedTileSetIndex]->getName().c_str() : "Select TileSet")) {
        for (int i = 0; i < static_cast<int>(tileSets.size()); ++i) {
            bool isSelected = (i == _selectedTileSetIndex);
            if (ImGui::Selectable(tileSets[i]->getName().c_str(), isSelected)) {
                std::cout << "Selected TileSet: " << tileSets[i]->getName() << std::endl;
                _selectedTileSetIndex = i;
                updateObjectSelector(tileSets);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Object selector
    for (int i = 0; i < static_cast<int>(_objects.size()); ++i) {
        if (ImGui::Selectable(_objects[i].c_str(), i == _selectedIndex)) {
            _selectedIndex = i;
            if (_onObjectSelected) {
                _onObjectSelected(_objects[i]);
            }
        }
    }

    ImGui::End();
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

void ObjectSelector::setOnObjectSelected(std::function<void(const std::string&)> callback) {
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
    addObject("Erase");

    if (_selectedTileSetIndex >= 0 && _selectedTileSetIndex < static_cast<int>(tileSets.size())) {
        const auto& selectedTileSet = tileSets[_selectedTileSetIndex];
        for (int j = 0; j < static_cast<int>(selectedTileSet->getTileCount()); ++j) {
            const auto& tile = selectedTileSet->getTile(j);
            addObject(std::to_string(tile.getId()));
        }
    }
}
