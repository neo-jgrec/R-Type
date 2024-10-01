#ifndef SERVERPLAYER_HPP
    #define SERVERPLAYER_HPP

#include <asio.hpp>
#include <vector>

#include "../../core/network/NetworkService.hpp"
#include "../../game/Player.hpp"

class ServerPlayer: public Player {
private:
    const asio::ip::udp::endpoint& _endpoint;
    uint64_t _lastTimeConnected{};

public:
    ServerPlayer(const asio::ip::udp::endpoint& endpoint, const uint8_t id, const std::pair<uint32_t, uint32_t> &position = {0, 0})
        : Player(id, position), _endpoint(endpoint) {}

    void sendHitCollision(NetworkingService networkingService) const;

    void sendPlayerDied(NetworkingService networkingService, const std::vector<ServerPlayer> &players) const;

    void sendPlayerShoot(NetworkingService networkingService, const std::vector<ServerPlayer> &players) const;
    void sendPlayerLoadShoot(NetworkingService networkingService, const std::vector<ServerPlayer> &players) const;
    void sendPlayerMove(NetworkingService networkingService,const std::vector<ServerPlayer> &players) const;
};

#endif //SERVERPLAYER_HPP
