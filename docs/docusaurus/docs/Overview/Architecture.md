# Project Architecture Overview

## Introduction
The **R-Type** project is a multiplayer shoot'em-up game developed using a client-server architecture. This document provides an overview of how different components interact with each other.

## System Overview
The system is composed of the following major parts:
- **Client**: Handles the rendering, input management, and communicates with the server.
- **Server**: Manages the game state, handles networking, and synchronizes the game world for all clients.
- **Game Engine**: Core engine that powers the game logic, entity management, rendering, and other systems.
- **Network**: Manages the communication between client and server, sending game events like player movements.
- **Map Editor**: A tool to create and modify maps that can be used in the game.

## Key Concepts
- **ECS (Entity-Component-System)**: The architecture used for organizing game objects and behavior.
- **UDP Networking**: Communication between the client and the server is handled via the UDP protocol.
- **Event System**: Events are used to synchronize actions between clients and the server.
