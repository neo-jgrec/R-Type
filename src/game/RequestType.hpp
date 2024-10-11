#pragma once

enum RequestType {
    PlayerConnect = 0,
    PlayerDisconnect = 1,
    GameStart = 2,
    GameOver = 3,
    MapScroll = 4,
    TileDestroy = 5,
    PlayerShoot = 6,
    PlayerMove = 7,
    PlayerCollide = 8,
    PlayerHit = 9,
    PlayerDie = 10,
    EnemySpawn = 11,
    EnemyMove = 12,
    EnemyDie = 13,
};
