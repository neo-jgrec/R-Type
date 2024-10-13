---
sidebar_position: 1
---

# Introduction

Welcome to the Developer Documentation for the Game Server. This documentation provides detailed information on the architecture, setup, and usage of the server components. It is intended for developers who are working on or contributing to the game server.

## Overview

The code is separate in 4 parts:
- **Components**: Contains the components that can be attached to entities.
- **EntityFactory**: Contains the factory that creates entities.
- **Systems**: Contains the systems that handle the game logic.
- **Server**: Contains the server that manages the game logic and links the other parts together.

## Getting Started

The server is mostly only based on the Game Engine / ECS and the network library. This parts will not be detailed here.

The main loop is separated in 2 parts:
- **Connection**: The server waits for clients to connect.
- **Game**: The server runs the game logic.

Each of this part is run in loop every second.
