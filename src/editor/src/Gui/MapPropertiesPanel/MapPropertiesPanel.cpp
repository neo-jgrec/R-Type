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
    std::string backgroundPath = map.getBackgroundPath();
    int backgroundHeight = map.getBackgroundHeight();

    if (ImGui::InputInt("Width", &width)) {
        if (width < 1)
            width = 1;
        map.setWidth(width);
        if (_onPropertyChanged) _onPropertyChanged();
    }
    if (ImGui::InputInt("Height", &height)) {
        if (height < 1)
            height = 1;
        map.setHeight(height);
        if (_onPropertyChanged) _onPropertyChanged();
    }
    if (ImGui::InputInt("Cell Size", &cellSize)) {
        if (cellSize < 1)
            cellSize = 1;
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
        if (backgroundHeight < 1)
            backgroundHeight = 1;
        map.setBackgroundHeight(backgroundHeight);
        if (_onPropertyChanged) _onPropertyChanged();
    }

    ImGui::End();
}

void MapPropertiesPanel::setOnPropertyChanged(std::function<void()> callback) {
    _onPropertyChanged = std::move(callback);
}
