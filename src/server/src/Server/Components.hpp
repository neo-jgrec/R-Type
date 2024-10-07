#ifndef COMPONENTS_HPP
    #define COMPONENTS_HPP

#include "../../../core/network/NetworkService.hpp"

struct Network {
    NetworkingService &service = NetworkingService::getInstance();
};

enum CollisionMask {
    WORLD = 0b0001,
    PLAYER = 0b0010,
    ENEMY = 0b0100
};

struct World {
    uint8_t speed = 0;
    uint32_t scroll = 0;

    std::pair<uint32_t, uint32_t> size;
    std::vector<std::vector<uint8_t>> tiles;
};

struct Player {
    // asio::ip::udp::endpoint& endpoint;
    uint8_t id = 0;
};

struct Enemy {
    uint8_t id = 0;
};


struct Projectile {};

#endif //COMPONENTS_HPP
