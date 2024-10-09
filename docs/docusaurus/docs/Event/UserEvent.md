
```markdown
# Event Class

## Overview
The `Event` class is like a message that contains information about something that happened in the game, such as a player moving, a player sending a chat message, or an entity appearing on the screen. Each event has a **type** and, depending on the type, different kinds of information.

## What Is an Event?

An event has two important parts:

- **Type**: This tells us what kind of event it is (e.g., a player moving, a chat message).
- **Payload**: This is the extra information that goes along with the event, like which player moved and where.

## How to Create an Event

Creating an `Event` is like writing a little note about something that happened. Here’s how to create different types of events:

### Example 1: A Player Moves

Let’s say we want to create an event that says player 1 moved to a new position:

```cpp
PlayerMovement movement = {1, 100.0f, 200.0f, 0.0f}; // Player 1 moves to (100, 200, 0)
Event moveEvent(0x01, movement);
```

### Example 2: A Player Sends a Chat Message

If a player sends a message in the game, we can create an event like this:

```cpp
ChatMessage chat = {1, "Hello, world!"}; // Player 1 says "Hello, world!"
Event chatEvent(0x15, chat);
```

### Example 3: A New Entity Appears

If a new enemy or object appears in the game, we can create an event for that:

```cpp
EntitySpawn spawn = {42, 50.0f, 75.0f}; // An entity with ID 42 spawns at (50, 75)
Event spawnEvent(0x09, spawn);
```

## Checking the Event Type

Once you have an event, you might want to know what kind of event it is:

```cpp
if (moveEvent.getType() == 0x01) {
    std::cout << "This event is about a player moving!" << std::endl;
}
```

## Getting the Event’s Information

To get the details of what happened in an event, you can use `getPayload()`:

```cpp
auto payload = moveEvent.getPayload();

if (std::holds_alternative<PlayerMovement>(payload)) {
    PlayerMovement movement = std::get<PlayerMovement>(payload);
    std::cout << "Player ID: " << movement.playerId
              << " moved to X: " << movement.x
              << " Y: " << movement.y
              << " Z: " << movement.z << std::endl;
}
```

## Types of Events

Here are some of the types of events you might encounter:

- **Player Movement (0x01)**: When a player moves somewhere.
- **Chat Message (0x15)**: When a player sends a message in the chat.
- **Player Shoot (0x05)**: When a player fires a weapon.
- **Player Health Update (0x06)**: When a player's health changes.
- **Entity Spawn (0x09)**: When a new entity appears in the game.
- **Entity Destroy (0x0A)**: When an entity is removed from the game.

## Summary

The `Event` class helps you track what’s happening in the game. It lets you know when players move, chat, or when entities appear or disappear. Using events makes it easier to keep track of what’s going on and respond to changes in the game.

---

Happy coding! 🎮
```

