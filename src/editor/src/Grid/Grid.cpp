/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Grid
*/

#include "Grid.hpp"
#include <iostream>
using namespace Editor;

Grid::Grid(int width, int height, int cellSize)
    : _width(width), _height(height), _cellSize(cellSize) {
    defineGrid();
}

void Grid::draw(sf::RenderWindow &window) {
    window.draw(&_gridLines[0], _gridLines.size(), sf::Lines);
    window.draw(&_vertices[0], _vertices.size(), sf::Lines);
}

void Grid::updateGrid(float zoomLevel) {
    //_gridLines.clear();
    //_cellSize = static_cast<int>(zoomLevel * 10);
    //if (_cellSize < 5)
    //    _cellSize = 5;
    //defineGrid();
}

void Grid::setGridSize(int width, int height) {
    _width = width;
    _height = height;

    defineGrid();
}

void Grid::defineGrid() {
    sf::Color gridColor(255, 255, 255, 50);

    _gridLines.clear();
    for (float x = 0; x <= _width; x += _cellSize) {
        _gridLines.push_back(sf::Vertex(sf::Vector2f(x, 0), gridColor));
        _gridLines.push_back(sf::Vertex(sf::Vector2f(x, _height), gridColor));
    }
    for (float y = 0; y <= _height; y += _cellSize) {
        _gridLines.push_back(sf::Vertex(sf::Vector2f(0, y), gridColor));
        _gridLines.push_back(sf::Vertex(sf::Vector2f(_width, y), gridColor));
    }
    _vertices.clear();

    _vertices.push_back(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));
    _vertices.push_back(sf::Vertex(sf::Vector2f(_width, 0), sf::Color::Red));

    _vertices.push_back(sf::Vertex(sf::Vector2f(_width, _height), sf::Color::Red));
    _vertices.push_back(sf::Vertex(sf::Vector2f(0, _height), sf::Color::Red));

    _vertices.push_back(sf::Vertex(sf::Vector2f(0, _height), sf::Color::Red));
    _vertices.push_back(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));

    _vertices.push_back(sf::Vertex(sf::Vector2f(_width, 0), sf::Color::Red));
    _vertices.push_back(sf::Vertex(sf::Vector2f(_width, _height), sf::Color::Red));
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
