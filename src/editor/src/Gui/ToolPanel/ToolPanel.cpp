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
        {"Eraser", Tool::ERASER},
        {"Selector", Tool::SELECTOR}
    };
}

void ToolPanel::render() {
    ImGui::Begin("Tools");

    for (const auto& tool : _tools) {
        //if (_selectedTool == tool.second)
        //    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 1.0f, 1.0f));
        if (ImGui::Button(tool.first.c_str())) {
            if (_selectedTool == tool.second)
                _selectedTool = Tool::NONE;
            else
                _selectedTool = tool.second;
            if (_onToolSelected)
                _onToolSelected(_selectedTool);
        }
        //if (_selectedTool == tool.second)
        //    ImGui::PopStyleColor();
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
