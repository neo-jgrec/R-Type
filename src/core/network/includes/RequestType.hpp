/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RequestType
*/


#pragma once

#ifndef REQUESTTYPE_HPP
    #define REQUESTTYPE_HPP
    #include <cstdint> // Pour les types uint8_t, uint16_t, uint64_t


// Enum pour les types de message GDTP
enum class GDTPMessageType : uint8_t {
    PlayerMovement = 0x01,
    GameEvent = 0x02,
    EntityUpdate = 0x03,
    Acknowledgment = 0x04,
    PlayerShoot = 0x05,
    PlayerHealthUpdate = 0x06,
    PlayerRespawn = 0x07,
    PlayerDisconnect = 0x08,
    EntitySpawn = 0x09,
    EntityDestroy = 0x0A,
    PowerUpCollected = 0x0B,
    GameStart = 0x0C,
    GameOver = 0x0D,
    PingRequest = 0x0E,
    PingResponse = 0x0F,
    ConnectionRequest = 0x10,
    ConnectionAccept = 0x11,
    ConnectionReject = 0x12,
    Synchronization = 0x13,
    ErrorMessage = 0x14,
    ChatMessage = 0x15,
    GamePause = 0x16,
    MapData = 0x20,
    MapScroll = 0x21
};
#endif //REQUESTTYPE_HPP
