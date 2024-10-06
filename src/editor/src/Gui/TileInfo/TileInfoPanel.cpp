/*
** EPITECH PROJECT, 2024
** editor
** File description:
** TileInfo
*/

#include "TileInfoPanel.hpp"
#include <imgui.h>

using namespace Editor;

TileInfoPanel::TileInfoPanel() = default;

void TileInfoPanel::render(Map& map, std::vector<sf::Vector2i>& selectedTiles) {
    ImGui::Begin("Tile Information");

    for (const auto& selectedTile : selectedTiles) {
        ImGui::Text("Tile ID: %d", map.getTile(selectedTile.x, selectedTile.y));
    }

    ImGui::End();
}
