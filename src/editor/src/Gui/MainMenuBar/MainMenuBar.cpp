/*
** EPITECH PROJECT, 2024
** editor
** File description:
** MainMenuBar
*/

#include "MainMenuBar.hpp"

using namespace Editor;

MainMenuBar::MainMenuBar() {}

void MainMenuBar::render() {
    if (ImGui::BeginMainMenuBar()) {
        renderFileMenu();
        renderEditMenu();
        renderViewMenu();
        ImGui::EndMainMenuBar();
    }
}

void MainMenuBar::renderFileMenu() const {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New Map") && onNewMap) onNewMap();
        if (ImGui::MenuItem("Open Map") && onOpenMap) onOpenMap();
        if (ImGui::MenuItem("Save Map") && onSaveMap) onSaveMap();
        if (ImGui::MenuItem("Load TileSet") && onLoadTileSet) onLoadTileSet();
        ImGui::Separator();
        if (ImGui::MenuItem("Exit") && onExit) onExit();
        ImGui::EndMenu();
    }
}

void MainMenuBar::renderEditMenu() const {
    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Undo", "Ctrl+Z") && onUndo) onUndo();
        if (ImGui::MenuItem("Redo", "Ctrl+Y") && onRedo) onRedo();
        ImGui::EndMenu();
    }
}

void MainMenuBar::renderViewMenu() const {
    if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Reset View") && onResetView) onResetView();
        ImGui::EndMenu();
    }
}
