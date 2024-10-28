#ifndef SERVER_HPP
#define SERVER_HPP

#include <shared_mutex>

#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../core/network/NetworkService.hpp"
#include "../../../core/ecs/Entity/Entity.hpp"
#include "../../../core/config/ConfigManager.hpp"

enum GameState: uint8_t {
    STARTING,
    WAITING_CONNECTION,
    LOBBY,
    GAME,
    STOPPING,
};

class Server {
private:
    core::GameEngine _gameEngine{false};
    NetworkingService _networkingService{1111};
    ConfigManager _configManager;

    core::ecs::Entity _world;
    std::array<std::optional<std::shared_ptr<asio::ip::udp::endpoint>>, 4> _playersConnection;
    std::array<std::optional<core::ecs::Entity>, 4> _players;

    GameState _gameState = STARTING;

    mutable std::shared_mutex registry_mutex;

    void update();

public:
    Server();

    core::GameEngine &getGameEngine() { return _gameEngine; }
    NetworkingService &getNetworkingService() { return _networkingService; }
    ConfigManager &getConfigManager() { return _configManager; }
    std::array<std::optional<std::shared_ptr<asio::ip::udp::endpoint>>, 4> &getPlayersConnection() { return _playersConnection; }
    std::array<std::optional<core::ecs::Entity>, 4> &getPlayers() { return _players; }
    std::shared_mutex &getRegistryMutex() const { return registry_mutex; }

    void setGameState(const GameState gameState) { _gameState = gameState; }

    void start();
    void run();

    bool asPlayerConnected();

    void sendRequestToPlayers(uint8_t requestType, const std::vector<uint8_t> &payload);
    void sendRequestToPlayers(uint8_t requestType, const std::vector<uint8_t> &payload, uint8_t selfPlayer);
};

#endif //SERVER_HPP
