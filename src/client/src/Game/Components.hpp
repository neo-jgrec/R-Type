#pragma once

#include <SFML/Graphics.hpp>

struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

struct Drawable {
    sf::RectangleShape shape;
};

struct ClockComponent {
    sf::Clock clock;
    sf::Time elapsedTime;
};

struct EventComponent {
    sf::Event event;
};
