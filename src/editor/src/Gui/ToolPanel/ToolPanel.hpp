/*
** EPITECH PROJECT, 2024
** editor
** File description:
** ToolPanel
*/

#ifndef TOOLPANEL_HPP_
#define TOOLPANEL_HPP_

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <functional>
#include <vector>
#include <string>

namespace Editor {
    enum class Tool {
        NONE,
        ERASER,
        SELECTOR
    };

    class ToolPanel {
    public:
        ToolPanel();
        void render();
        [[nodiscard]]
        Tool getSelectedTool() const;
        void setOnToolSelected(std::function<void(Tool)> callback);

    private:
        std::vector<std::pair<std::string, Tool>> _tools;
        Tool _selectedTool;
        std::function<void(Tool)> _onToolSelected;
    };
}

#endif /* !TOOLPANEL_HPP_ */
