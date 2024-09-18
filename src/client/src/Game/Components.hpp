#pragma once

#include <SFML/Graphics.hpp>

struct KeyBinding {
    sf::Keyboard::Key moveUpKey = sf::Keyboard::Z;
    sf::Keyboard::Key moveDownKey = sf::Keyboard::S;
    sf::Keyboard::Key moveLeftKey = sf::Keyboard::Q;
    sf::Keyboard::Key moveRightKey = sf::Keyboard::D;
    sf::Keyboard::Key fireKey = sf::Keyboard::Space;
};

struct InputStateComponent {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool fire = false;
};

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
