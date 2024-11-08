#include "EventFactory.hpp"

#include <cstdint>
#include <SFML/System/Vector2.hpp>

#include "src/event/Event.hpp"
#include "../../../game/RequestType.hpp"


const std::unordered_map<uint8_t, EventFactory::EventHandler> EventFactory::handlers = {
    {RequestType::PlayerConnect, handlePlayerConnect},
    {RequestType::PlayerDisconnect, handlePlayerDisconnect},
    {RequestType::GameStart, handleGameStart},
    {RequestType::GameOver, handleGameOver},
    {RequestType::MapScroll, handleMapScroll},
    {RequestType::TileDestroy, handleTileDestroy},
    {RequestType::PlayerProjectileCreate, handlePlayerProjectileCreate},
    {RequestType::PlayerMissileCreate, handlePlayerMissileCreate},
    {RequestType::PlayerProjectileDestroy, handlePlayerProjectileDestroy},
    {RequestType::PlayerMissileDestroy, handlePlayerMissileDestroy},
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
    if (const auto it = handlers.find(header.messageType); it != handlers.end()) {
        return it->second(header, payload);
    }

    throw EventPool::UnknownEvent(header.messageType);
}

Event EventFactory::handleMapScroll([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 4) {
        throw std::runtime_error("Invalid payload size for MapScroll event");
    }
    std::uint32_t mapScroll = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    return {RequestType::MapScroll, header, mapScroll};
}

Event EventFactory::handleTileDestroy([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 8) {
        throw std::runtime_error("Invalid payload size for TileDestroy event");
    }
    std::uint32_t x = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    std::uint32_t y = (payload[4] << 24) | (payload[5] << 16) | (payload[6] << 8) | payload[7];
    return {RequestType::TileDestroy, header, sf::Vector2u{x, y}};
}

Event EventFactory::handlePlayerProjectileCreate([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 9) {
        throw std::runtime_error("Invalid payload size for PlayerProjectileCreate event");
    }
    std::uint8_t projectileId = payload[0];
    std::uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
    std::uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
    return {RequestType::PlayerProjectileCreate, header, std::make_pair(projectileId, sf::Vector2u{x, y})};
}

Event EventFactory::handlePlayerMissileCreate([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 9) {
        throw std::runtime_error("Invalid payload size for PlayerMissileCreate event");
    }
    std::uint8_t missileId = payload[0];
    std::uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
    std::uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
    return {RequestType::PlayerMissileCreate, header, std::make_pair(missileId, sf::Vector2u{x, y})};
}

Event EventFactory::handlePlayerProjectileDestroy([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 1) {
        throw std::runtime_error("Invalid payload size for PlayerProjectileDestroy event");
    }
    std::uint8_t projectileId = payload[0];
    return {RequestType::PlayerProjectileDestroy, header, projectileId};
}

Event EventFactory::handlePlayerMissileDestroy([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 1) {
        throw std::runtime_error("Invalid payload size for PlayerMissileDestroy event");
    }
    std::uint8_t missileId = payload[0];
    return {RequestType::PlayerMissileDestroy, header, missileId};
}

Event EventFactory::handlePlayerMove([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 9) {
        throw std::runtime_error("Invalid payload size for PlayerMove event");
    }
    std::uint8_t playerId = payload[0];
    std::uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
    std::uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
    return {RequestType::PlayerMove, header, std::make_pair(playerId, sf::Vector2u{x, y})};
}

Event EventFactory::handlePlayerCollide([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 9) {
        throw std::runtime_error("Invalid payload size for PlayerCollide event");
    }
    std::uint8_t playerId = payload[0];
    std::uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
    std::uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
    return {RequestType::PlayerCollide, header, std::make_pair(playerId, sf::Vector2u{x, y})};
}

Event EventFactory::handlePlayerHit([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 1) {
        throw std::runtime_error("Invalid payload size for PlayerHit event");
    }
    std::uint8_t playerId = payload[0];
    return {RequestType::PlayerHit, header, playerId};
}

Event EventFactory::handlePlayerDie([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 1) {
        throw std::runtime_error("Invalid payload size for PlayerDie event");
    }
    std::uint8_t playerId = payload[0];
    return {RequestType::PlayerDie, header, playerId};
}

Event EventFactory::handleEnemySpawn([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 10) {
        throw std::runtime_error("Invalid payload size for EnemySpawn event");
    }
    std::uint8_t enemyId = payload[0];
    std::uint8_t enemyType = payload[1];
    std::uint32_t x = (payload[2] << 24) | (payload[3] << 16) | (payload[4] << 8) | payload[5];
    std::uint32_t y = (payload[6] << 24) | (payload[7] << 16) | (payload[8] << 8) | payload[9];
    return {RequestType::EnemySpawn, header, std::make_tuple(enemyId, enemyType, sf::Vector2u{x, y})};
}

Event EventFactory::handleEnemyMove([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 9) {
        throw std::runtime_error("Invalid payload size for EnemyMove event");
    }
    std::uint8_t enemyId = payload[0];
    std::uint32_t x = (payload[1] << 24) | (payload[2] << 16) | (payload[3] << 8) | payload[4];
    std::uint32_t y = (payload[5] << 24) | (payload[6] << 16) | (payload[7] << 8) | payload[8];
    return {RequestType::EnemyMove, header, std::make_pair(enemyId, sf::Vector2u{x, y})};
}

Event EventFactory::handleEnemyDie([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 1) {
        throw std::runtime_error("Invalid payload size for EnemyDie event");
    }
    std::uint8_t enemyId = payload[0];
    return {RequestType::EnemyDie, header, enemyId};
}

Event EventFactory::handlePlayerConnect([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 1) {
        throw std::runtime_error("Invalid payload size for PlayerConnect event");
    }
    std::uint8_t playerId = payload[0];
    return {RequestType::PlayerConnect, header, playerId};
}

Event EventFactory::handlePlayerDisconnect([[maybe_unused]] const GDTPHeader& header, const std::vector<uint8_t>& payload)
{
    if (payload.size() != 1) {
        throw std::runtime_error("Invalid payload size for PlayerDisconnect event");
    }
    std::uint8_t playerId = payload[0];
    return {RequestType::PlayerDisconnect, header, playerId};
}

Event EventFactory::handleGameStart([[maybe_unused]] const GDTPHeader& header, [[maybe_unused]] const std::vector<uint8_t>& payload)
{
    if (!payload.empty()) {
        throw std::runtime_error("Invalid payload size for GameStart event");
    }
    return {RequestType::GameStart, header};
}

Event EventFactory::handleGameOver([[maybe_unused]] const GDTPHeader& header, [[maybe_unused]] const std::vector<uint8_t>& payload)
{
    if (!payload.empty()) {
        throw std::runtime_error("Invalid payload size for GameOver event");
    }
    return {RequestType::GameOver, header};
}
