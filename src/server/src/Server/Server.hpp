#ifndef SERVER_HPP
#define SERVER_HPP

#include <utility>

#include "../../../core/ecs/Registry/Registry.hpp"
#include "../../../core/ecs/Entity/Entity.hpp"


class Server {
private:
    core::ecs::Registry _registry;
    core::ecs::Entity _world;
    std::vector<core::ecs::Entity> _players;
    std::vector<core::ecs::Entity> _enemies;

    void init();
    void update();
public:
    Server(core::ecs::Registry registry)
        : _registry(std::move(registry))
    {
        init();
    }

    void run();
};



#endif //SERVER_HPP
