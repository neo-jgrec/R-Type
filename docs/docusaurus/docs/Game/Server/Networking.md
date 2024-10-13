---
sidebar_position: 2
---

# Event handling

The server interact in two ways with the events:
- **Receiving events**: The server receives events from the clients and the systems.
- **Sending events**: The server sends events to the clients and the systems.

Event receiving from players are send to all the players.

## Receiving events

- **PlayerConnect** event: The server receives this event when a player connects to the server.
- **PlayerDisconnect** event: The server receives this event when a player disconnects from the server.
- **GameStart** event: The server receives this event when the game starts.
- **PlayerMove** event: The server receives this event when a player moves.
- **PlayerShoot** event: The server receives this event when a player shoots.

## Sending events

- **PlayerConnect** event: The server sends this event when a player connects to the server.
- **PlayerDisconnect** event: The server sends this event when a player disconnects from the server.
- **GameStart** event: The server sends this event when the game starts.
- **MapScroll** event: The server sends this event when the map scrolls.
- **TileDestroy** event: The server sends this event when a tile is destroyed.
- **PlayerMove** event: The server sends this event when a player moves.
- **PlayerShoot** event: The server sends this event when a player shoots.
- **PlayerCollide** event: The server sends this event when a player collides with an object.
- **PlayerHit** event: The server sends this event when a player is hit.
- **PlayerDie** event: The server sends this event when a player dies.
- **EnemySpawn** event: The server sends this event when an enemy spawns.
- **EnemyMove** event: The server sends this event when an enemy moves.
- **EnemyDie** event: The server sends this event when an enemy dies.
