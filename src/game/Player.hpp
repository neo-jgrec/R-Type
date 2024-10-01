#ifndef PLAYER_HPP
    #define PLAYER_HPP

#include <cstdint>
#include <utility>

enum class Orientation {
    UP = 0b00,
    DOWN = 0b01,
    LEFT = 0b10,
    RIGHT = 0b11
};

class Player {
protected:
    uint8_t _id;
    std::pair<uint8_t, uint8_t> _position;
    Orientation _orientation;

public:
    Player(const uint8_t id, const std::pair<uint32_t, uint32_t> &position = {0, 0})
        : _id(id), _position(position) {}

    [[nodiscard]] uint8_t getId() const {return _id;}

};

#endif //PLAYER_HPP
