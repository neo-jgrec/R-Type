/*
** EPITECH PROJECT, 2024
** editor
** File description:
** MapPropertiesPanel
*/

#include "MapPropertiesPanel.hpp"

using namespace Editor;

MapPropertiesPanel::MapPropertiesPanel() {}

void MapPropertiesPanel::render(Map& map) {
    ImGui::Begin("Map Properties");

    static char mapName[256];
    strncpy(mapName, map.getName().c_str(), sizeof(mapName));
    mapName[sizeof(mapName) - 1] = '\0';
    if (ImGui::InputText("Map Name", mapName, sizeof(mapName))) {
        map.setName(mapName);
        if (_onPropertyChanged) _onPropertyChanged();
    }

    int width = map.getWidth();
    int height = map.getHeight();
    int cellSize = map.getGrid().getCellSize();

    if (ImGui::InputInt("Width", &width)) {
        map.setWidth(width);
        if (_onPropertyChanged) _onPropertyChanged();
    }
    if (ImGui::InputInt("Height", &height)) {
        map.setHeight(height);
        if (_onPropertyChanged) _onPropertyChanged();
    }
    if (ImGui::InputInt("Cell Size", &cellSize)) {
        map.getGrid().setCellSize(cellSize);
        if (_onPropertyChanged) _onPropertyChanged();
    }

    ImGui::End();
}

void MapPropertiesPanel::setOnPropertyChanged(std::function<void()> callback) {
    _onPropertyChanged = std::move(callback);
}
