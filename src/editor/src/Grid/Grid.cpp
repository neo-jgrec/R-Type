/*
** EPITECH PROJECT, 2024
** editor
** File description:
** Grid
*/

#include "Grid.hpp"
#include <iostream>
using namespace Editor;

Grid::Grid(float width, float height, float cellSize)
    : _width(width), _height(height), _cellSize(cellSize) {
    for (float x = 0; x <= width; x += cellSize) {
        _gridLines.push_back(sf::Vertex(sf::Vector2f(x, 0), sf::Color::White));
        _gridLines.push_back(sf::Vertex(sf::Vector2f(x, height), sf::Color::White));
    }
    for (float y = 0; y <= height; y += cellSize) {
        _gridLines.push_back(sf::Vertex(sf::Vector2f(0, y), sf::Color::White));
        _gridLines.push_back(sf::Vertex(sf::Vector2f(width, y), sf::Color::White));
    }

    _vertices.push_back(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));
    _vertices.push_back(sf::Vertex(sf::Vector2f(width, 0), sf::Color::Red));

    _vertices.push_back(sf::Vertex(sf::Vector2f(width, height), sf::Color::Red));
    _vertices.push_back(sf::Vertex(sf::Vector2f(0, height), sf::Color::Red));

    _vertices.push_back(sf::Vertex(sf::Vector2f(0, height), sf::Color::Red));
    _vertices.push_back(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));

    _vertices.push_back(sf::Vertex(sf::Vector2f(width, 0), sf::Color::Red));
    _vertices.push_back(sf::Vertex(sf::Vector2f(width, height), sf::Color::Red));
}

void Grid::draw(sf::RenderWindow &window) {
    window.draw(&_gridLines[0], _gridLines.size(), sf::Lines);
    window.draw(&_vertices[0], _vertices.size(), sf::Lines);
}

void Grid::updateGrid(float zoomLevel) {
    _gridLines.clear();
    _cellSize = zoomLevel * 10;
    if (_cellSize < 5.0f)
        _cellSize = 5.0f;
    for (float x = 0; x <= _width; x += _cellSize) {
        _gridLines.push_back(sf::Vertex(sf::Vector2f(x, 0), sf::Color::White));
        _gridLines.push_back(sf::Vertex(sf::Vector2f(x, _height), sf::Color::White));
    }
    for (float y = 0; y <= _height; y += _cellSize) {
        _gridLines.push_back(sf::Vertex(sf::Vector2f(0, y), sf::Color::White));
        _gridLines.push_back(sf::Vertex(sf::Vector2f(_width, y), sf::Color::White));
    }
}
