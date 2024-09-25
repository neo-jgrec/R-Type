#include "Server.hpp"

Server::Server(const std::string &serverAddress, const uint16_t serverPort): _networkingService(serverAddress, serverPort)
{
    _networkingService.run();
    std::cout << "Server is running and listening on all interfaces (" << serverAddress << ":" << serverPort << ")" << std::endl;
}

Server::~Server()
{
    _networkingService.stop();

    std::cout << "Server stopped" << std::endl;
}

void Server::connectPlayer(const asio::ip::udp::endpoint &endpoint)
{
    const uint8_t playerId = _players.size();
    _players.emplace_back(endpoint, playerId);

    std::cout << "Player " << playerId << " connected" << std::endl;
}

void Server::update()
{
    _world.updateScroll(1);
    _world.sendMapScrolling();
}
