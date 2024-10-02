/*
** EPITECH PROJECT, 2024
** editor
** File description:
** ToolPanel
*/

#include "ToolPanel.hpp"

using namespace Editor;

ToolPanel::ToolPanel() : _selectedTool(Tool::NONE) {
    _tools = {
        {"Eraser", Tool::ERASER}
    };
}

void ToolPanel::render() {
    ImGui::Begin("Tools");

    for (const auto& tool : _tools) {
        if (ImGui::Button(tool.first.c_str())) {
            _selectedTool = tool.second;
            if (_onToolSelected) {
                _onToolSelected(_selectedTool);
            }
        }
        ImGui::SameLine();
    }

    ImGui::End();
}

Tool ToolPanel::getSelectedTool() const {
    return _selectedTool;
}

void ToolPanel::setOnToolSelected(std::function<void(Tool)> callback) {
    _onToolSelected = std::move(callback);
}
