#ifndef SERVER_HPP
    #define SERVER_HPP

#include "ServerPlayer.hpp"
#include "ServerWorld.hpp"

class Server {
private:
    NetworkingService _networkingService;

    ServerWorld _world;
    std::vector<ServerPlayer> _players;

public:
    Server(const std::string &serverAddress, uint16_t serverPort);
    ~Server();

    void connectPlayer(const asio::ip::udp::endpoint& endpoint);
    void update();
};

#endif //SERVER_HPP
