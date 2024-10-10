#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../core/network/NetworkService.hpp"
#include "../../../core/ecs/Entity/Entity.hpp"

class Server {
private:
    core::GameEngine _gameEngine{false};
    NetworkingService _networkingService{1111};

    core::ecs::Entity _connectionHub;
    core::ecs::Entity _world;

    std::array<std::optional<core::ecs::Entity>, 4> _players;
    std::vector<core::ecs::Entity> _enemies;

    bool _asGameStarted = false;
    bool _isRunning = true;

    bool asPlayerConnected();

    void update();
public:
    Server();

    void run();
};

#endif //SERVER_HPP
