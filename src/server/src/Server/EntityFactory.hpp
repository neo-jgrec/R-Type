#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include "Server.hpp"

class EntityFactory {
public:
    static core::ecs::Entity createWorld(Server &server, const std::string& filePath);
    static core::ecs::Entity createPlayer(Server &server, const asio::ip::udp::endpoint &endpoint, uint8_t id);
    static core::ecs::Entity createEnemy(Server &server,  uint32_t x);
    static core::ecs::Entity createProjectile(Server &server, const core::ecs::Entity &player, uint8_t id);
    static core::ecs::Entity createTile(Server &server, const std::pair<uint32_t, uint32_t> &size, uint32_t x, uint32_t y);
};

#endif //ENTITYFACTORY_HPP
