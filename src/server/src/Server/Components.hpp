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
};

struct Player {
    std::shared_ptr<asio::ip::udp::endpoint> endpoint;
    uint8_t id;
    uint8_t health;
    time_t lastTimePacketReceived;
};

struct Enemy {
    uint8_t id;
};

struct Projectile {
    uint8_t id;
};

struct Tile {};

#endif //COMPONENTS_HPP
