---
sidebar_position: 2
---

# Event handling

The server interact in two ways with the events:
- **Receiving events**: The server receives events from the clients and the systems.
- **Sending events**: The server sends events to the clients and the systems.

Event receiving from players are send to all the players.
- **Exception**: The event `PlayerConnect` is send of the new player with the same header to know is own id.

## Receiving events

- **PlayerConnect** When a player connects to the server.
- **PlayerDisconnect** When a player disconnects from the server.
- **GameStart** When a player starts the game.
- **PlayerMove** When a player moves.
- **PlayerProjectileShoot** When a player shoots a projectile.
- **PlayerMissileShoot** When a player shoots a missile.

## Sending events

- **PlayerConnect** When a player connects to the server.
- **PlayerDisconnect** When a player disconnects from the server.
- **GameStart** When the game starts or a player joins the game after it started.
- **MapScroll** When the game starts or a player joins the game after it started, send the current scroll of the map.
- **TileDestroy** When a tile is destroyed.
- **PlayerMove** When a player moves.
- **PlayerProjectileCreate** When a player creates a projectile.
- **PlayerMissileCreate** When a player creates a missile.
- **PlayerProjectileDestroy** When a projectile is destroyed.
- **PlayerMissileDestroy** When a missile is destroyed.
- **PlayerCollide** When a player collide.
- **PlayerHit** When a player is hit.
- **PlayerDie** When a player dies.
- **EnemySpawn** When an enemy spawns.
- **EnemyMove** When an enemy moves.
- **EnemyDie** When an enemy dies.
