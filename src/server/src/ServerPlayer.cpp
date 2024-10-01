#include "ServerPlayer.hpp"

#include "../../game/PacketType.hpp"

void ServerPlayer::sendHitCollision(NetworkingService networkingService) const{
    const std::vector payload = {
        static_cast<uint8_t>(_position.first >> 24),
        static_cast<uint8_t>(_position.first >> 16),
        static_cast<uint8_t>(_position.first >> 8),
        static_cast<uint8_t>(_position.first),
        static_cast<uint8_t>(_position.second >> 24),
        static_cast<uint8_t>(_position.second >> 16),
        static_cast<uint8_t>(_position.second >> 8),
        static_cast<uint8_t>(_position.second)
    };

    std::cout << "Player " << _id << " was hit" << std::endl;
    networkingService.sendRequest(
        _endpoint,
        HitCollision,
        payload);
}

void ServerPlayer::sendPlayerDied(NetworkingService networkingService, const std::vector<ServerPlayer> &players) const{
    const std::vector payload = {
        static_cast<uint8_t>(_id)
    };

    std::cout << "Player " << _id << " died" << std::endl;
    for (const auto &player : players)
        networkingService.sendRequest(
            player._endpoint,
            PlayerDied,
            payload);
}

void ServerPlayer::sendPlayerShoot(NetworkingService networkingService, const std::vector<ServerPlayer> &players) const{
    const std::vector payload = {
        static_cast<uint8_t>(_id),
        static_cast<uint8_t>(_position.first >> 24),
        static_cast<uint8_t>(_position.first >> 16),
        static_cast<uint8_t>(_position.first >> 8),
        static_cast<uint8_t>(_position.first),
        static_cast<uint8_t>(_position.second >> 24),
        static_cast<uint8_t>(_position.second >> 16),
        static_cast<uint8_t>(_position.second >> 8),
        static_cast<uint8_t>(_position.second)
    };

    std::cout << "Player " << _id << " shot" << std::endl;
    for (const auto &player : players) {
        if (player.getId() == _id)
            continue;
        networkingService.sendRequest(
            player._endpoint,
            PlayerShoot,
            payload);
    }
}

void ServerPlayer::sendPlayerLoadShoot(NetworkingService networkingService, const std::vector<ServerPlayer> &players) const{
    const std::vector payload = {
        static_cast<uint8_t>(_id)
    };

    std::cout << "Player " << _id << " loaded a shoot" << std::endl;
    for (const auto &player : players) {
        if (player.getId() == _id)
            continue;
        networkingService.sendRequest(
            player._endpoint,
            PlayerLoadShoot,
            payload);
    }
}

void ServerPlayer::sendPlayerMove(NetworkingService networkingService, const std::vector<ServerPlayer> &players) const{
    const std::vector payload = {
        static_cast<uint8_t>(_id),
        static_cast<uint8_t>(_orientation),
        static_cast<uint8_t>(_position.first >> 24),
        static_cast<uint8_t>(_position.first >> 16),
        static_cast<uint8_t>(_position.first >> 8),
        static_cast<uint8_t>(_position.first),
        static_cast<uint8_t>(_position.second >> 24),
        static_cast<uint8_t>(_position.second >> 16),
        static_cast<uint8_t>(_position.second >> 8),
        static_cast<uint8_t>(_position.second)
    };

    std::cout << "Player " << _id << " moved" << std::endl;
    for (const auto &player : players) {
        if (player.getId() == _id)
            continue;
        networkingService.sendRequest(
            player._endpoint,
            PlayerMovement,
            payload);
    }
}
