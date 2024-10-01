/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventTypeMapping
*/

#pragma once

#define EVENT_TYPE_LIST \
X(PlayerMovement, 0x01) \
X(GameEvent, 0x02) \
X(EntityUpdate, 0x03) \
X(Acknowledgment, 0x04) \
X(PlayerShoot, 0x05) \
X(PlayerHealthUpdate, 0x06) \
X(PlayerRespawn, 0x07) \
X(PlayerDisconnect, 0x08) \
X(EntitySpawn, 0x09) \
X(EntityDestroy, 0x0A) \
X(PowerUpCollected, 0x0B) \
X(GameStart, 0x0C) \
X(GameOver, 0x0D) \
X(PingRequest, 0x0E) \
X(PingResponse, 0x0F) \
X(ConnectionRequest, 0x10) \
X(ConnectionAccept, 0x11) \
X(ConnectionReject, 0x12) \
X(Synchronization, 0x13) \
X(ErrorMessage, 0x14) \
X(ChatMessage, 0x15) \
X(GamePause, 0x16) \
X(MapData, 0x20) \
X(MapScroll, 0x21)

