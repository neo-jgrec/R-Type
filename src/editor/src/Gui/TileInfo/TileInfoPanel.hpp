/*
** EPITECH PROJECT, 2024
** editor
** File description:
** TileInfo
*/

#ifndef TILEINFOPANEL_HPP_
#define TILEINFOPANEL_HPP_

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <string>
#include "../../Map/Map.hpp"

namespace Editor {
    class TileInfoPanel {
    public:
        TileInfoPanel();
        static void render(Map& map, std::vector<sf::Vector2i>& selectedTiles);

    private:
        std::string _tileInfo;
    };
}

#endif /* !TILEINFOPANEL_HPP_ */