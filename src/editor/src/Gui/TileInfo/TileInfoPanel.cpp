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

    for (size_t i = 0; i < selectedTiles.size(); ++i) {
        const auto& selectedTile = selectedTiles[i];
        Tile& tile = const_cast<Tile&>(map.getTileObject(selectedTile.x, selectedTile.y));

        ImGui::PushID(static_cast<int>(i));

        ImGui::Text("Tile ID: %d", tile.getId());
        ImGui::Text("Position: %d, %d", selectedTile.x, selectedTile.y);
        ImGui::Spacing();
        bool isDestructible = tile.isDestructible();
        if (ImGui::Checkbox("Is Destructible", &isDestructible)) {
            tile.setDestructible(isDestructible);
            if (isDestructible)
                tile.setStyle(TileStyle::DESTRUCTIBLE);
            else
                tile.setStyle(TileStyle::DEFAULT);
        }
        ImGui::Spacing();
        ImGui::Text("Style:");
        const char* styleItems[] = { "Default", "Destructible", "Spawn" };
        int currentStyle = static_cast<int>(tile.getStyle());
        if (ImGui::Combo("Style", &currentStyle, styleItems, IM_ARRAYSIZE(styleItems))) {
            tile.setStyle(static_cast<TileStyle>(currentStyle));
        }
        ImGui::Spacing();
        ImGui::Text("Tags:");
        for (const auto& tag : tile.getTags()) {
            ImGui::BulletText("%s", tag.c_str());
            ImGui::SameLine();
            if (ImGui::SmallButton("Remove")) {
                tile.removeTag(tag);
            }
        }

        static char newTag[256] = "";
        ImGui::InputText("New Tag", newTag, IM_ARRAYSIZE(newTag));
        ImGui::SameLine();
        if (ImGui::Button("Add Tag") && newTag[0] != '\0') {
            tile.addTag(newTag);
            newTag[0] = '\0';
        }

        ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::End();
}
