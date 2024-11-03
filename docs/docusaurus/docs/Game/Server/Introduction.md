---
sidebar_position: 1
---

# Introduction

Welcome to the Developer Documentation for the Game Server. This documentation provides detailed information on the architecture, setup, and usage of the server components. It is intended for developers who are working on or contributing to the game server.

## Purpose

The game server is the core of the game. It manages the game logic, the entities, the components, and the systems. It also manages the network communication between the clients and the server.

The server is based on the Game Engine / ECS and the network library.

:::info
The server as always the authority on the game. It means that the server is the one that decides what happens in the game. The clients are only here to display the game and send the inputs to the server.
:::

## Overview

The code is separate in 8 parts:
Only server files:
- **Components**: Contains the components that can be attached to entities.
    - `Server/Components.hpp`
- **EntityFactory**: Contains the factory that creates entities.
    - `Server/EntityFactory.hpp`
    - `Server/EntityFactory.cpp`
- **Systems**: Contains the systems that handle the game logic.
    - `Server/Systems.hpp`
    - `Server/Systems.cpp`
- **EventsFactory**: Contains the handler for the events.
    - `Server/EventsFactory.hpp`
    - `Server/EventsFactory.cpp`
- **Server**: Contains the server that manages the game logic and links the other parts together.
    - `Server/Server.hpp`
    - `Server/Server.cpp`
- **main**: Contains the main function that runs the server.
    - `main.cpp`
Common files between the server and the clients:
- **CollisionMask**: Contains the collision mask that are used by the server and the clients.
    - `CollisionMask.hpp`
- **RequestType**: Contains the request type that are used by the server and the clients.
    - `RequestType.hpp`

## Getting Started

The server is mostly only based on the Game Engine / ECS and the network library. This parts will not be detailed here.

The main loop is separated in 2 parts:
- **STARTING**: The server waits for clients to connect.
- **WAITING_CONNECTION**: The server waits for a client to connect.
- **LOBBY**: The server waits for other clients to connect and one of them to start the game.
- **GAME**: The server runs the game logic.
- **STOPPING**: The server stops the game and waits for clients to disconnect.

Each of this part is run in loop depending of the computer speed (the server will wait 1ms between each loop to avoid using 100% of the CPU).

## Initialization

The server initializes the systems and set the event handlers in the constructor. The server is started by calling the `run` method.

## Command line interface

The game engine has a command line interface that can be used to interact with the server.
Two commands are added by the server:

- **stop**: Change the state of the server to STOPPING.
- **kick**: Kick a player from the server.
