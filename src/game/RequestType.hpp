#pragma once

#include <cstdint>

enum RequestType: uint8_t {
    PlayerConnect = 0,
    PlayerDisconnect = 1,
    GameStart = 2,
    GameOver = 3,
    MapScroll = 4,
    TileDestroy = 5,
    PlayerProjectileShoot = 6,
    PlayerProjectileCreate = 7,
    PlayerProjectileDestroy = 8,
    PlayerMissileShoot = 9,
    PlayerMissileCreate = 10,
    PlayerMissileDestroy = 11,
    PlayerMove = 12,
    PlayerCollide = 13,
    PlayerHit = 14,
    PlayerDie = 15,
    EnemySpawn = 16,
    EnemyMove = 17,
    EnemyDie = 18,
};
