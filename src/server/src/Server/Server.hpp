#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../core/ecs/Entity/Entity.hpp"

class Server {
private:
    core::GameEngine _gameEngine;

    core::ecs::Entity _world;
    std::vector<core::ecs::Entity> _players;
    std::vector<core::ecs::Entity> _enemies;

    void update();
public:
    Server();

    void run();
};

#endif //SERVER_HPP
