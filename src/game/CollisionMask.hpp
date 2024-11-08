#ifndef COLLISIONMASK_HPP
#define COLLISIONMASK_HPP

enum CollisionMask {
    WORLD = 0b00001,
    PLAYER = 0b00010,
    ENEMY = 0b00100,
    TILE = 0b01000,
    PLAYER_PROJECTILE = 0b10000,
    PLAYER_MISSILE = 0b100000,
    BALL = 0b1000000,
    WORLD_BORDER = 0b10000000
};

#endif //COLLISIONMASK_HPP
