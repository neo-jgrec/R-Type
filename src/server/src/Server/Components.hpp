#ifndef COMPONENTS_HPP
    #define COMPONENTS_HPP

#include "../../../core/network/NetworkService.hpp"

struct Network {
    NetworkingService &service;
};

struct World {
    time_t lastTimeEnemySpawned;
    time_t enemySpawnRate;

    std::pair<uint32_t, uint32_t> size;
    uint8_t tileSize;
    std::vector<std::pair<uint32_t, uint32_t>> spawnPoints;
};

struct Player {
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
