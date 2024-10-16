#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../core/network/NetworkService.hpp"
#include "../../../core/ecs/Entity/Entity.hpp"

class Server {
private:
    core::GameEngine _gameEngine{false};
    NetworkingService _networkingService{1111};

    std::array<std::optional<core::ecs::Entity>, 4> _players;

    bool _asGameStarted = false;
    bool _isRunning = true;

    bool asPlayerConnected();

    void update();
public:
    Server();

    core::GameEngine &getGameEngine() { return _gameEngine; }
    NetworkingService &getNetworkingService() { return _networkingService; }

    void run();

    void sendRequestToPlayers(uint8_t requestType, const std::vector<uint8_t> &payload);
};

#endif //SERVER_HPP
