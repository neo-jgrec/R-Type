#include <cstdint>
#include <iostream>
#include <vector>

enum {

};

class World {
protected:
    uint8_t _scrollSpeed;
    uint32_t _scroll;

public:
    World(const uint8_t scrollSpeed = 0, const uint32_t scroll = 0)
        : _scrollSpeed(scrollSpeed), _scroll(scroll) {}

    [[nodiscard]] uint8_t getScrollSpeed() const {return _scrollSpeed;}
    void setScrollSpeed(const uint8_t scrollSpeed) {_scrollSpeed = scrollSpeed;}

    [[nodiscard]] uint32_t getScroll() const {return _scroll;}
    void setScroll(const uint32_t scroll) {_scroll = scroll;}

    void updateScroll(const uint32_t delta) {_scroll += _scrollSpeed * delta;}
};

class ServerWorld: public World {
public:
    ServerWorld(const uint8_t scrollSpeed = 0, const uint32_t scroll = 0)
        : World(scrollSpeed, scroll) {}

    void sendMapScrolling() const {
        const std::vector payload = {
            static_cast<uint8_t>(_scroll >> 24),
            static_cast<uint8_t>(_scroll >> 16),
            static_cast<uint8_t>(_scroll >> 8),
            static_cast<uint8_t>(_scroll)
        };

        // TODO: send payload to clients
        std::cout << "Sending payload: " << payload[0] << payload[1] << payload[2] << payload[3] << std::endl;
    }
};


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

class ServerPlayer: public Player {
public:
    ServerPlayer(const uint8_t id, const std::pair<uint32_t, uint32_t> &position = {0, 0})
        : Player(id, position) {}

    void sendHitCollision() const
    {
        const std::vector payload = {
            static_cast<uint8_t>(_position.first >> 24),
            static_cast<uint8_t>(_position.first >> 16),
            static_cast<uint8_t>(_position.first >> 8),
            static_cast<uint8_t>(_position.first),
            static_cast<uint8_t>(_position.second >> 24),
            static_cast<uint8_t>(_position.second >> 16),
            static_cast<uint8_t>(_position.second >> 8),
            static_cast<uint8_t>(_position.second)
        };

        // TODO: send payload to client
    }

    void sendPlayerDied(std::vector<Player> &players) const
    {
        const std::vector payload = {
            static_cast<uint8_t>(_id)
        };

        // TODO: send payload to clients
    }

    //
    // Other player actions received from clients
    //

    void sendPlayerShoot(std::vector<Player> &players) const
    {
        const std::vector payload = {
            static_cast<uint8_t>(_id),
            static_cast<uint8_t>(_position.first >> 24),
            static_cast<uint8_t>(_position.first >> 16),
            static_cast<uint8_t>(_position.first >> 8),
            static_cast<uint8_t>(_position.first),
            static_cast<uint8_t>(_position.second >> 24),
            static_cast<uint8_t>(_position.second >> 16),
            static_cast<uint8_t>(_position.second >> 8),
            static_cast<uint8_t>(_position.second)
        };
    }

    void sendPlayerLoadShoot(std::vector<Player> &players) const
    {
        const std::vector payload = {
            static_cast<uint8_t>(_id)
        };

        // TODO: send payload to clients
    }

    void sendPlayerMove(std::vector<Player> &players) const
    {
        const std::vector payload = {
            static_cast<uint8_t>(_id),
            static_cast<uint8_t>(_orientation),
            static_cast<uint8_t>(_position.first >> 24),
            static_cast<uint8_t>(_position.first >> 16),
            static_cast<uint8_t>(_position.first >> 8),
            static_cast<uint8_t>(_position.first),
            static_cast<uint8_t>(_position.second >> 24),
            static_cast<uint8_t>(_position.second >> 16),
            static_cast<uint8_t>(_position.second >> 8),
            static_cast<uint8_t>(_position.second)
        };

        // TODO: send payload to clients
    }
};

int main()
{
    std::cout << "un serveur, mais vreeeument" << std::endl;
    return 0;
}
