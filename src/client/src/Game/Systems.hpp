#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "Game.hpp"

namespace Systems {
    void playerInput(Game &game);
    void playerMovement(Game &game);
    void gameEvent(Game &game);
    void gameView(Game &game);
    void hitAnimation(Game &game);
    void enemyIndicatorSystem(Game &game);
    void enemyIndicatorRenderSystem(Game &game);
};

#endif //SYSTEMS_HPP
