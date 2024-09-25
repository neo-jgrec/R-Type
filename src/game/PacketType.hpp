#ifndef PACKETTYPE_HPP
    #define PACKETTYPE_HPP

enum PacketType {
    ConnectionRequest = 0,
    ConnectionResponse = 1,
    PlayerMovement = 2,
    PlayerShoot = 3,
    PlayerLoadShoot = 4,
    PlayerDied = 5,
    MapScrolling = 6,
    HitCollision = 7
};

#endif //PACKETTYPE_HPP
