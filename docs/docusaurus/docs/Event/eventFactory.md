
```markdown
# EventFactory Class

## Overview
The `EventFactory` class is responsible for creating `Event` objects from received network data. It translates `GDTPHeader` information and raw payload data into structured `Event` instances that the game server or client can process. This class encapsulates the logic needed to interpret the various event types specified in the `GDTP` protocol.

## Header File: `EventFactory.hpp`

```cpp
/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventFactory.hpp
*/

#pragma once

#ifndef EVENTFACTORY_HPP
#define EVENTFACTORY_HPP

#include "Event.hpp"
#include "../core/network/includes/RequestHeader.hpp"
#include <map>
#include <functional>
#include <vector>
#include <stdexcept>

/**
 * @class EventFactory
 * @brief Responsible for translating network data into Event objects.
 *
 * The `EventFactory` processes incoming network packets and creates corresponding `Event` objects.
 * It uses a map to associate message types with handler functions, making it easy to add or update event handlers.
 */
class EventFactory {
public:
    /**
     * @brief Constructs an EventFactory and initializes the handler map.
     *
     * The constructor sets up the handler map, associating each message type with its corresponding handler function.
     * This allows for dynamic handling of different message types.
     */
    EventFactory();

    /**
     * @brief Creates an `Event` object from the provided GDTP header and payload.
     * @param header The `GDTPHeader` containing metadata for the event.
     * @param payload The raw binary data associated with the event.
     * @return An `Event` object corresponding to the message type in the header.
     * @throws std::runtime_error if the message type is unknown or if the payload size is incorrect.
     *
     * @code
     * GDTPHeader header = ...; // Constructed or received earlier
     * std::vector<uint8_t> payload = ...; // Received payload data
     * EventFactory factory;
     * try {
     *     Event event = factory.createEvent(header, payload);
     *     // Process the event as needed
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Failed to create event: " << e.what() << std::endl;
     * }
     * @endcode
     */
    Event createEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload);

private:
    std::map<uint8_t, std::function<Event(const GDTPHeader&, const std::vector<uint8_t>&)>> eventHandlers;

    Event handlePlayerMovement(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    Event handlePlayerShoot(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    Event handleChatMessage(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    Event handlePlayerHealthUpdate(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    Event handleEntitySpawn(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    Event handleEntityDestroy(const GDTPHeader& header, const std::vector<uint8_t>& payload);
};

#endif // EVENTFACTORY_HPP
```

## Source File: `EventFactory.cpp`

```cpp
/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventFactory.cpp
*/

#include "EventFactory.hpp"
#include <cstring>
#include <stdexcept>

/**
 * @brief Constructs an `EventFactory` and sets up the handler map.
 *
 * This constructor initializes the map of event handlers, associating each message type with a specific function
 * to handle the creation of that event type.
 */
EventFactory::EventFactory() {
    eventHandlers[0x01] = [this](const GDTPHeader& header, const std::vector<uint8_t>& payload) {
        return handlePlayerMovement(header, payload);
    };
    eventHandlers[0x05] = [this](const GDTPHeader& header, const std::vector<uint8_t>& payload) {
        return handlePlayerShoot(header, payload);
    };
    eventHandlers[0x15] = [this](const GDTPHeader& header, const std::vector<uint8_t>& payload) {
        return handleChatMessage(header, payload);
    };
    eventHandlers[0x06] = [this](const GDTPHeader& header, const std::vector<uint8_t>& payload) {
        return handlePlayerHealthUpdate(header, payload);
    };
    eventHandlers[0x09] = [this](const GDTPHeader& header, const std::vector<uint8_t>& payload) {
        return handleEntitySpawn(header, payload);
    };
    eventHandlers[0x0A] = [this](const GDTPHeader& header, const std::vector<uint8_t>& payload) {
        return handleEntityDestroy(header, payload);
    };
}

/**
 * @brief Creates an Event object from the provided GDTPHeader and payload.
 *
 * @param header The GDTPHeader containing event metadata.
 * @param payload The raw binary data of the event.
 * @return A fully constructed Event object.
 * @throws std::runtime_error if the event type is unsupported.
 */
Event EventFactory::createEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    uint8_t type = header.messageType;

    if (eventHandlers.find(type) != eventHandlers.end()) {
        return eventHandlers[type](header, payload);
    }

    throw std::runtime_error("Unknown event type: " + std::to_string(type));
}

/**
 * @brief Handles the creation of a PlayerMovement event.
 *
 * @param header The GDTPHeader object.
 * @param payload The payload data containing the PlayerMovement information.
 * @return An Event containing a PlayerMovement payload.
 */
Event EventFactory::handlePlayerMovement(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    if (payload.size() < sizeof(PlayerMovement)) {
        throw std::runtime_error("Invalid payload size for PlayerMovement");
    }

    PlayerMovement movement;
    std::memcpy(&movement.playerId, &payload[0], 4);
    std::memcpy(&movement.x, &payload[4], 4);
    std::memcpy(&movement.y, &payload[8], 4);
    std::memcpy(&movement.z, &payload[12], 4);

    return Event(header.messageType, movement);
}

// Similar implementations for the other handlers...
```

## Usage Examples

### Creating an Event from Network Data

```cpp
// Suppose you have received a GDTPHeader and a payload from the network
GDTPHeader header = ...; // Received header
std::vector<uint8_t> payload = ...; // Received payload

// Create an instance of EventFactory
EventFactory factory;

try {
    // Create an Event using the factory
    Event event = factory.createEvent(header, payload);

    // Process the event as needed...
} catch (const std::runtime_error& e) {
    std::cerr << "Failed to create event: " << e.what() << std::endl;
}
```

### Adding a Custom Event Handler

If you need to add a custom event handler, modify the `EventFactory` class as follows:

```cpp
// Add a new entry to the eventHandlers map in the constructor.
eventHandlers[0x0B] = [this](const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    return handlePowerUpCollected(header, payload);
};

// Implement the new handler function.
Event EventFactory::handlePowerUpCollected(const GDTPHeader& header, const std::vector<uint8_t>& payload) {
    // Validate payload size and extract data...
    // Create and return the Event.
}
```

## Error Handling

- Throws `std::runtime_error` if an unknown event type is encountered.
- Validates the payload size before creating each event, ensuring that the payload matches the expected size for each event type.

## Key Features

- **Dynamic Event Handling**: Uses a map to dynamically associate message types with event handler functions.
- **Ease of Extension**: New event types can be added by simply defining a new handler function and adding it to the map.
- **Encapsulation**: Keeps event creation logic separate from other parts of the system, promoting a cleaner architecture.
```
