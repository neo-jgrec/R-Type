/*
** EPITECH PROJECT, 2024
** editor
** File description:
** MapPropertiesPanel
*/

#ifndef MAPPROPERTIESPANEL_HPP_
#define MAPPROPERTIESPANEL_HPP_

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <functional>
#include "../../Map/Map.hpp"

namespace Editor {
    class MapPropertiesPanel {
    public:
        MapPropertiesPanel();
        void render(Map& map);
        void setOnPropertyChanged(std::function<void()> callback);

    private:
        std::function<void()> _onPropertyChanged;
    };
}

#endif /* !MAPPROPERTIESPANEL_HPP_ */
