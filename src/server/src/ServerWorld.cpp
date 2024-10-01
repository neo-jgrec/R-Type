#include "ServerWorld.hpp"

#include <iostream>
#include <vector>

void ServerWorld::sendMapScrolling() const {
    const std::vector payload = {
        static_cast<uint8_t>(_scroll >> 24),
        static_cast<uint8_t>(_scroll >> 16),
        static_cast<uint8_t>(_scroll >> 8),
        static_cast<uint8_t>(_scroll)
    };

    // TODO: send payload to clients
    std::cout << "Sending payload: " << payload[0] << payload[1] << payload[2] << payload[3] << std::endl;
}
