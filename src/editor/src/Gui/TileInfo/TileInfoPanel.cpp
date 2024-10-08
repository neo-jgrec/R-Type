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
        Tile& tile = const_cast<Tile&>(map.getTileObject(selectedTile.x, selectedTile.y));
        ImGui::Text("Tile ID: %d", tile.getId());
        ImGui::Text("Position: %d, %d", selectedTile.x, selectedTile.y);
        bool isDestructible = tile.isDestructible();
        if (ImGui::Checkbox("Is Destructible", &isDestructible)) {
            tile.setDestructible(isDestructible);
        }
        ImGui::Separator();
    }

    ImGui::End();
}
