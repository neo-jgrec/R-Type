#pragma once

#include <cstdint>

enum MessageType {
    PlayerMovement = 0x01,        // Mise à jour de la position du joueur
    PlayerShoot = 0x05,           // Envoi d'un missile ou arme par le joueur
    PlayerHealthUpdate = 0x06,    // Mise à jour de la santé du joueur
    PlayerRespawn = 0x07,         // Réapparition d'un joueur
    PlayerDisconnect = 0x08,      // Déconnexion d'un joueur
    EntityUpdate = 0x03,          // Mise à jour d'une entité
    EntitySpawn = 0x09,           // Création d'une nouvelle entité
    EntityDestroy = 0x0A,         // Destruction d'une entité
    GameEvent = 0x02,             // Envoi d'un événement spécifique en jeu
    PowerUpCollected = 0x0B,      // Ramassage d'un power-up par un joueur
    GameStart = 0x0C,             // Début de la partie
    GameOver = 0x0D,              // Fin de la partie
    PingRequest = 0x0E,           // Demande de ping pour vérifier la connexion
    PingResponse = 0x0F,          // Réponse à la demande de ping
    Synchronization = 0x13,       // Synchronisation de l'état du jeu
    Acknowledgment = 0x04,        // Accusé de réception d'un paquet
    ErrorMessage = 0x14,          // Notification d'une erreur
    ChatMessage = 0x15,           // Envoi de message de chat entre joueurs
    GamePause = 0x16,             // Mise en pause du jeu
    MapData = 0x20,               // Recevoir la map
    MapScroll = 0x21 ,             // Avancer dans la map
    ConnectionRequest = 0x10,     // Requête de connexion d'un client
    ConnectionAccept = 0x11,      // Acceptation de la connexion par le serveur
    ConnectionReject = 0x12,      // Rejet de la connexion
};