# Client Overview

The **Client** in the R-Type project is responsible for rendering the game world, handling user input, and sending relevant events (such as player movement) to the server. It runs its own game loop and renders graphics using **SFML**.

## Key Responsibilities
- Rendering game entities (players, enemies, projectiles).
- Handling keyboard input to control the player.
- Sending and receiving game events from the server.

## Main Classes
- **Game**: Main class responsible for managing the game loop, rendering, and updating game entities.
- **Event**: Used for sending and receiving data between client and server.
