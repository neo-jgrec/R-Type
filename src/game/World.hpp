#ifndef WORLD_HPP
    #define WORLD_HPP

#include <cstdint>

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

#endif //WORLD_HPP
