/*
** EPITECH PROJECT, 2024
** editor
** File description:
** TileInfo
*/

#include "TileInfoPanel.hpp"
#include <imgui.h>
#include <iostream>

using namespace Editor;

TileInfoPanel::TileInfoPanel() {}

void TileInfoPanel::render(Map& map, std::vector<sf::Vector2i>& selectedTiles) {
    ImGui::Begin("Tile Information");

    std::cout << "Selected Tiles: " << selectedTiles.size() << std::endl;

    for (const auto& selectedTile : selectedTiles) {
        ImGui::Text("Tile ID: %d", map.getTile(selectedTile.x, selectedTile.y));
    }

    ImGui::End();
}
