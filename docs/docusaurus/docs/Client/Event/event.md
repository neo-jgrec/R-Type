
---

# Event Class Documentation

## Overview

The `Event` class represents a game event that can occur, such as player movements, chat messages, player actions, and more. Each event type is associated with a specific payload containing the relevant data for that event. This class uses `std::variant` to manage different payload types, ensuring type safety while handling events.

## Data Structures

### PlayerMovement

```cpp
struct PlayerMovement {
    uint32_t playerId; ///< The ID of the player.
    float x; ///< The X-coordinate of the player's new position.
    float y; ///< The Y-coordinate of the player's new position.
    float z; ///< The Z-coordinate of the player's new position.
    
    bool operator==(const PlayerMovement& other) const;
};
```

- **Description**: Represents the data for a player movement event.
- **Fields**:
   - `playerId`: The unique ID of the player.
   - `x`, `y`, `z`: Coordinates of the player's new position.
- **Equality Operator**: Compares two `PlayerMovement` objects. Returns `true` if the `playerId` and all coordinates match.

---

### ChatMessage

```cpp
struct ChatMessage {
    uint32_t playerId; ///< The ID of the player sending the message.
    std::string message; ///< The content of the chat message.
    
    bool operator==(const ChatMessage& other) const;
};
```

- **Description**: Represents the data for a chat message event.
- **Fields**:
   - `playerId`: The ID of the player who sent the message.
   - `message`: The content of the message.
- **Equality Operator**: Compares two `ChatMessage` objects. Returns `true` if both `playerId` and `message` are identical.

---

### Other Event Structures

Additional event types follow a similar pattern:

- `PlayerShoot`
- `PowerUpCollected`
- `EntityUpdate`
- `EntitySpawn`
- `EntityDestroy`
- `PlayerHealthUpdate`
- `NoData` (Represents events without a payload)

Each structure has fields specific to its event type, and an equality operator for comparison.

---

## Event Class

```cpp
class Event {
public:
    template <typename T>
    Event(uint8_t type, const T& payload);

    uint8_t getType() const;

    const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData>& getPayload() const;

    bool operator==(const Event& other) const;
    
private:
    uint8_t type; ///< The type of the event.
    std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData> payload; ///< The payload of the event.
};
```

### Methods

#### Constructor

```cpp
template <typename T>
Event(uint8_t type, const T& payload);
```

- **Description**: Constructs an `Event` object with a specified payload.
- **Parameters**:
   - `type`: The type of the event (e.g., player movement, chat message).
   - `payload`: The payload associated with the event, matching the type.
- **Example**:
  ```cpp
  PlayerMovement movement{123, 10.0f, 20.0f, 5.0f};
  Event playerMoveEvent(0x01, movement);
  ```

#### getType

```cpp
uint8_t getType() const;
```

- **Description**: Returns the type of the event.
- **Returns**: The type as a `uint8_t`.
- **Example**:
  ```cpp
  uint8_t eventType = playerMoveEvent.getType();
  ```

#### getPayload

```cpp
const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, PowerUpCollected, EntityUpdate, PlayerHealthUpdate, EntitySpawn, EntityDestroy, NoData>& getPayload() const;
```

- **Description**: Retrieves the payload associated with the event.
- **Returns**: A `std::variant` containing the payload.
- **Example**:
  ```cpp
  auto payload = playerMoveEvent.getPayload();
  if (std::holds_alternative<PlayerMovement>(payload)) {
      PlayerMovement movement = std::get<PlayerMovement>(payload);
      std::cout << "Player moved to (" << movement.x << ", " << movement.y << ", " << movement.z << ")" << std::endl;
  }
  ```

#### Equality Operator

```cpp
bool operator==(const Event& other) const;
```

- **Description**: Compares two `Event` objects for equality.
- **Parameters**:
   - `other`: The other `Event` object to compare.
- **Returns**: `true` if both events have the same type and payload, `false` otherwise.
- **Example**:
  ```cpp
  Event event1(0x01, movement1);
  Event event2(0x01, movement2);
  if (event1 == event2) {
      std::cout << "The events are identical." << std::endl;
  }
  ```

---

## Usage Example

```cpp
#include "Event.hpp"
#include <iostream>

int main() {
    // Create a PlayerMovement event
    PlayerMovement movement{123, 100.0f, 200.0f, 300.0f};
    Event moveEvent(0x01, movement);

    // Access the event type
    uint8_t type = moveEvent.getType();
    std::cout << "Event type: " << static_cast<int>(type) << std::endl;

    // Access the payload
    auto payload = moveEvent.getPayload();
    if (std::holds_alternative<PlayerMovement>(payload)) {
        PlayerMovement movementData = std::get<PlayerMovement>(payload);
        std::cout << "Player ID: " << movementData.playerId << ", Position: (" 
                  << movementData.x << ", " << movementData.y << ", " << movementData.z << ")" << std::endl;
    }

    // Compare events
    PlayerMovement anotherMovement{123, 100.0f, 200.0f, 300.0f};
    Event anotherMoveEvent(0x01, anotherMovement);
    if (moveEvent == anotherMoveEvent) {
        std::cout << "The events are identical." << std::endl;
    }
}
```

---

## Notes

- The `Event` class and its associated structures ensure that game events are type-safe and easy to manipulate.
- `std::variant` allows for safe storage of different types of event data in a single object.
- The class design makes it easy to extend with new event types by adding new structs and updating the `Event` class accordingly.

---

