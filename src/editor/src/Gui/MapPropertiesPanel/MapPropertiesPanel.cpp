/*
** EPITECH PROJECT, 2024
** editor
** File description:
** MapPropertiesPanel
*/

#include "MapPropertiesPanel.hpp"

using namespace Editor;

MapPropertiesPanel::MapPropertiesPanel() = default;

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
    std::string backgroundPath = map.getBackgroundPath();
    int backgroundHeight = map.getBackgroundHeight();

    if (ImGui::InputInt("Width", &width)) {
        width = std::clamp(width, 1, 1000);
        map.setWidth(width);
        if (_onPropertyChanged) _onPropertyChanged();
    }
    if (ImGui::InputInt("Height", &height)) {
        height = std::clamp(height, 1, 1000);
        map.setHeight(height);
        if (_onPropertyChanged) _onPropertyChanged();
    }
    if (ImGui::InputInt("Cell Size", &cellSize)) {
        cellSize = std::clamp(cellSize, 1, 100);
        map.setCellSize(cellSize);
        if (_onPropertyChanged) _onPropertyChanged();
    }

    char backgroundPathBuffer[256];
    strncpy(backgroundPathBuffer, backgroundPath.c_str(), sizeof(backgroundPathBuffer));
    backgroundPathBuffer[sizeof(backgroundPathBuffer) - 1] = '\0';
    if (ImGui::InputText("Background Path", backgroundPathBuffer, sizeof(backgroundPathBuffer))) {
        map.setBackground(backgroundPathBuffer);
        if (_onPropertyChanged) _onPropertyChanged();
    }

    if (ImGui::InputInt("Background Height (in tiles)", &backgroundHeight)) {
        backgroundHeight = std::clamp(backgroundHeight, 1, map.getHeight());
        map.setBackgroundHeight(backgroundHeight);
        if (_onPropertyChanged) _onPropertyChanged();
    }

    ImGui::End();
}

void MapPropertiesPanel::setOnPropertyChanged(std::function<void()> callback) {
    _onPropertyChanged = std::move(callback);
}
