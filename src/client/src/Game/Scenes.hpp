#ifndef SCENES_HPP
#define SCENES_HPP

#include "Game.hpp"

namespace Scenes {
    void loadMainMenu(Game &game);
    void updateMainMenu(Game &game);

    void loadRoomMenu(Game &game);
    void updateRoomMenu(Game &game);

    void loadSettingsMenu(Game &game);
    void updateSettingsMenu(Game &game);

    void loadGame(Game &game);
    void updateGame(Game &game);

    std::vector<core::ecs::Entity> createWorldBorders(core::GameEngine& engine);
};

#endif //SCENES_HPP
