#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>

struct InputStateComponent {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool fire = false;
};

struct VelocityComponent {
    float dx, dy;
};

struct HealthComponent {
    int health;
};

struct DamageComponent {
    int damage;
};

struct ScoreComponent {
    int score;
};

// Identifiers

struct Player {};
struct Enemy {};
struct Projectile {};
