#ifndef COMPONENTS_HPP
    #define COMPONENTS_HPP

#include "../../../core/network/NetworkService.hpp"

enum OrientationEnum {
    UP = 0b00,
    DOWN = 0b01,
    LEFT = 0b10,
    RIGHT = 0b11
};

struct Network {
    NetworkingService &service = NetworkingService::getInstance();
};

struct Position {
    uint16_t x = 0;
    uint16_t y = 0;
};

struct Velocity {
    uint8_t x = 0;
    uint8_t y = 0;
};

struct Orientation {
    uint8_t orientation = 0;
};

struct RectangleCollider {
    uint16_t width = 0;
    uint16_t height = 0;
};

struct World {
    uint8_t speed = 0;
    uint32_t scroll = 0;
};

struct Player {
    asio::ip::udp::endpoint& endpoint;
    uint8_t id = 0;
};

struct Enemy {
    uint8_t id = 0;
};

enum ProjectileTeam {
    PLAYER,
    ENEMY,
    NEUTRAL
};

struct Projectile {
    ProjectileTeam team = NEUTRAL;
};

#endif //COMPONENTS_HPP
