#ifndef SERVERWORLD_HPP
    #define SERVERWORLD_HPP

#include "../../game/World.hpp"

class ServerWorld: public World {
public:
    ServerWorld(const uint8_t scrollSpeed = 0, const uint32_t scroll = 0)
        : World(scrollSpeed, scroll) {}

    void sendMapScrolling() const ;
};

#endif //SERVERWORLD_HPP
