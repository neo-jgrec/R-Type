#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../core/network/NetworkService.hpp"
#include "../../../core/ecs/Entity/Entity.hpp"
#include <shared_mutex>

class Server {
private:
    core::GameEngine _gameEngine{false};
    NetworkingService _networkingService{1111};

    core::ecs::Entity _world;
    std::array<std::optional<core::ecs::Entity>, 4> _players;

    bool _asGameStarted = false;
    bool _isRunning = true;

    mutable std::shared_mutex registry_mutex;

    void update();
public:
    Server();

    core::GameEngine &getGameEngine() { return _gameEngine; }
    NetworkingService &getNetworkingService() { return _networkingService; }
    std::array<std::optional<core::ecs::Entity>, 4> &getPlayers() { return _players; }
    std::shared_mutex &getRegistryMutex() const { return registry_mutex; }
    bool asGameStarted() const { return _asGameStarted; }

    void start();
    void run();

    bool asPlayerConnected();

    void sendRequestToPlayers(uint8_t requestType, const std::vector<uint8_t> &payload);
    void sendRequestToPlayers(uint8_t requestType, const std::vector<uint8_t> &payload, uint8_t selfPlayer);
};

#endif //SERVER_HPP
