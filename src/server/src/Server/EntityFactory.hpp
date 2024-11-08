#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include "Server.hpp"

namespace EntityFactory {
    core::ecs::Entity createWorld(Server &server, const std::string& filePath);
    core::ecs::Entity createPlayer(Server &server, uint8_t id);
    core::ecs::Entity createEnemy(Server &server,  uint32_t x, uint8_t enemyType = 0);
    core::ecs::Entity createProjectile(Server &server, const core::ecs::Entity &player);
    core::ecs::Entity createMissile(Server &server, const core::ecs::Entity &player);
    core::ecs::Entity createTile(Server &server, const std::pair<uint32_t, uint32_t> &size, uint32_t x, uint32_t y);
};

#endif //ENTITYFACTORY_HPP
