/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Grid
*/

#include "Grid.hpp"
using namespace Editor;

Grid::Grid(int width, int height, int cellSize)
    : _width(width), _height(height), _cellSize(cellSize) {
    defineGrid();
}

void Grid::draw(sf::RenderWindow &window, std::vector<sf::Vector2i>& selectedTiles) const {
    sf::Color highlightColor(0, 255, 0, 255); // Green color with some transparency
    std::vector<sf::Vertex> highlightVertices;

    for (const auto& tile : selectedTiles) {
        auto left = static_cast<float>(tile.x * _cellSize);
        auto top = static_cast<float>(tile.y * _cellSize);
        float right = left + static_cast<float>(_cellSize);
        float bottom = top + static_cast<float>(_cellSize);

        highlightVertices.emplace_back(sf::Vector2f(left, top), highlightColor);
        highlightVertices.emplace_back(sf::Vector2f(right, top), highlightColor);

        highlightVertices.emplace_back(sf::Vector2f(right, top), highlightColor);
        highlightVertices.emplace_back(sf::Vector2f(right, bottom), highlightColor);

        highlightVertices.emplace_back(sf::Vector2f(right, bottom), highlightColor);
        highlightVertices.emplace_back(sf::Vector2f(left, bottom), highlightColor);

        highlightVertices.emplace_back(sf::Vector2f(left, bottom), highlightColor);
        highlightVertices.emplace_back(sf::Vector2f(left, top), highlightColor);
    }

    window.draw(&_gridLines[0], _gridLines.size(), sf::Lines);
    window.draw(&_vertices[0], _vertices.size(), sf::Lines);
    if (!highlightVertices.empty())
        window.draw(&highlightVertices[0], highlightVertices.size(), sf::Lines);
}

void Grid::setGridSize(int width, int height) {
    _width = width;
    _height = height;

    defineGrid();
}

void Grid::defineGrid() {
    sf::Color gridColor(255, 255, 255, 50);

    _gridLines.clear();
    for (int x = 0; x <= _width; x += _cellSize) { // Changed float to int
        _gridLines.emplace_back(sf::Vector2f(static_cast<float>(x), 0), gridColor);
        _gridLines.emplace_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(_height)), gridColor);
    }
    for (int y = 0; y <= static_cast<int>(_height); y += static_cast<int>(_cellSize)) {
        _gridLines.emplace_back(sf::Vector2f(0, static_cast<float>(y)), gridColor);
        _gridLines.emplace_back(sf::Vector2f(static_cast<float>(_width), static_cast<float>(y)), gridColor);
    }
    _vertices.clear();

    _vertices.emplace_back(sf::Vector2f(0, 0), sf::Color::Red);
    _vertices.emplace_back(sf::Vector2f(static_cast<float>(_width), 0), sf::Color::Red);

    _vertices.emplace_back(sf::Vector2f(static_cast<float>(_width), static_cast<float>(_height)), sf::Color::Red);
    _vertices.emplace_back(sf::Vector2f(0, static_cast<float>(_height)), sf::Color::Red);

    _vertices.emplace_back(sf::Vector2f(0, static_cast<float>(_height)), sf::Color::Red);
    _vertices.emplace_back(sf::Vector2f(0, 0), sf::Color::Red);

    _vertices.emplace_back(sf::Vector2f(static_cast<float>(_width), 0), sf::Color::Red);
    _vertices.emplace_back(sf::Vector2f(static_cast<float>(_width), static_cast<float>(_height)), sf::Color::Red);
}

int Grid::getGridWidth() const {
    return _width;
}

int Grid::getGridHeight() const {
    return _height;
}

int Grid::getCellSize() const {
    return _cellSize;
}

void Grid::setCellSize(int cellSize) {
    _cellSize = cellSize;
}
