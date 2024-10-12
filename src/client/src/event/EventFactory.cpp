/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventFactory.cpp
*/

#include "EventFactory.hpp"
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <cstring>
#include "../../../game/RequestType.hpp"
#include "src/event/Event.hpp"

const std::unordered_map<uint8_t, EventFactory::EventHandler> EventFactory::handlers = {
    {RequestType::PlayerConnect, handlePlayerConnect},
    {RequestType::PlayerDisconnect, handlePlayerDisconnect},
    {RequestType::GameStart, handleGameStart},
    {RequestType::GameOver, handleGameOver},
    {RequestType::MapScroll, handleMapScroll},
    {RequestType::TileDestroy, handleTileDestroy},
    {RequestType::PlayerShoot, handlePlayerShoot},
    {RequestType::PlayerMove, handlePlayerMove},
    {RequestType::PlayerCollide, handlePlayerCollide},
    {RequestType::PlayerHit, handlePlayerHit},
    {RequestType::PlayerDie, handlePlayerDie},
    {RequestType::EnemySpawn, handleEnemySpawn},
    {RequestType::EnemyMove, handleEnemyMove},
    {RequestType::EnemyDie, handleEnemyDie},
};

Event EventFactory::createEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    uint8_t type = header.messageType;

    auto it = handlers.find(type);
    if (it != handlers.end()) {
        return it->second(header, payload);
    } else {
        throw EventPool::UnknownEvent(header.messageType);
    }
}

Event EventFactory::handleMapScroll([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint32_t mapScroll = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    return {RequestType::MapScroll, header, static_cast<int>(mapScroll)}; // TODO: change the var in the constructor to allow uint32_t
}

Event EventFactory::handleTileDestroy([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint32_t x = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    std::uint32_t y = (payload[4] << 24) | (payload[5] << 16) | (payload[6] << 8) | payload[7];
    return {RequestType::TileDestroy, header, sf::Vector2f{static_cast<float>(x), static_cast<float>(y)}};
}

Event EventFactory::handlePlayerShoot([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t projectileId = payload[0];
    return {RequestType::PlayerShoot, header, projectileId};
}

Event EventFactory::handlePlayerMove([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t playerId = payload[0];
    std::uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
    std::uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
    return {RequestType::PlayerMove, header, std::make_pair(playerId, sf::Vector2f{static_cast<float>(x), static_cast<float>(y)})};
}

Event EventFactory::handlePlayerCollide([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t playerId = payload[0];
    std::uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
    std::uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
    return {RequestType::PlayerCollide, header, std::make_pair(playerId, sf::Vector2f{static_cast<float>(x), static_cast<float>(y)})};
}

Event EventFactory::handlePlayerHit([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t playerId = payload[0];
    return {RequestType::PlayerHit, header, playerId};
}

Event EventFactory::handlePlayerDie([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t playerId = payload[0];
    return {RequestType::PlayerDie, header, playerId};
}

Event EventFactory::handleEnemySpawn([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t enemyId = payload[0];
    std::uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
    std::uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
    return {RequestType::EnemySpawn, header, std::make_pair(enemyId, sf::Vector2f{static_cast<float>(x), static_cast<float>(y)})};
}

Event EventFactory::handleEnemyMove([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t enemyId = payload[0];
    std::uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
    std::uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
    return {RequestType::EnemyMove, header, std::make_pair(enemyId, sf::Vector2f{static_cast<float>(x), static_cast<float>(y)})};
}

Event EventFactory::handleEnemyDie([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t enemyId = payload[0];
    return {RequestType::EnemyDie, header, enemyId};
}

Event EventFactory::handlePlayerConnect([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t playerId = payload[0];
    return {RequestType::PlayerConnect, header, playerId};
}

Event EventFactory::handlePlayerDisconnect([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    std::uint8_t playerId = payload[0];
    return {RequestType::PlayerDisconnect, header, playerId};
}

Event EventFactory::handleGameStart([[maybe_unused]] const GDTPHeader& header, [[maybe_unused]] const std::vector<uint8_t>& payload)
{
    return {RequestType::GameStart, header};
}

Event EventFactory::handleGameOver([[maybe_unused]] const GDTPHeader& header, [[maybe_unused]] const std::vector<uint8_t>& payload)
{
    return {RequestType::GameOver, header};
}
