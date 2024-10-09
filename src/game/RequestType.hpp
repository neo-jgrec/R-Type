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
    EnemyShoot = 12,
    EnemyDie = 13,
    OtherPlayerConnect = 14,
    OtherPlayerDisconnect = 15,
    OtherPlayerMove = 16,
    OtherPlayerShoot = 17,
    OtherPlayerHit = 18,
    OtherPlayerDie = 19,
};
