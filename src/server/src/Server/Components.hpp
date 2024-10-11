#ifndef COMPONENTS_HPP
    #define COMPONENTS_HPP

#include "../../../core/network/NetworkService.hpp"

struct Network {
    NetworkingService &service;
};

struct World {
    uint8_t speed = 0;
    uint32_t scroll = 0;

    std::pair<uint32_t, uint32_t> size;
    std::vector<std::vector<uint8_t>> tiles;
};

struct Player {
    const asio::ip::udp::endpoint &endpoint;
    uint8_t id;
    uint8_t health;
    time_t lastTimePacketReceived;
};

struct Enemy {
    uint8_t id;
};

struct Projectile {};

#endif //COMPONENTS_HPP
