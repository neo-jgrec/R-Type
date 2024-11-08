/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Grid
*/

#ifndef GRID_HPP_
#define GRID_HPP_

#include <SFML/Graphics.hpp>
#include <vector>
namespace Editor {
    class Grid {
        public:
            Grid(int width, int height, int cellSize);
            void draw(sf::RenderWindow &window, std::vector<sf::Vector2i>& selectedTiles) const;
            void setGridSize(int width, int height);

            [[nodiscard]]
            int getGridWidth() const;

            [[nodiscard]]
            int getGridHeight() const;

            [[nodiscard]]
            int getCellSize() const;

            void setCellSize(int cellSize);

        private:
            int _width;
            int _height;
            int _cellSize;
            std::vector<sf::Vertex> _vertices;
            std::vector<sf::Vertex> _gridLines;

            void defineGrid();
    };
}


#endif /* !GRID_HPP_ */
