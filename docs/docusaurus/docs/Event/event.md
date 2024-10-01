---

# Event System Documentation

The Event system manages the creation, storage, and processing of game and network events. It consists of several key components: the **Event** class, **EventPool** for managing events in a thread-safe manner, and **EventFactory** for dynamically creating events based on incoming data.

---

## EventFactory Class

### Overview

The **EventFactory** class is responsible for dynamically creating events based on the message type and payload received from the network. Instead of manually handling different event types in a switch statement, **EventFactory** abstracts the event creation logic into a single place, simplifying the code and making it more modular.

### Key Methods

#### `Event createEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload)`

This method accepts a **GDTPHeader** and a **payload** (a vector of bytes) and returns an **Event** object. Based on the `messageType` from the header, **EventFactory** determines which type of event to create and uses the payload to populate the event data.

##### Example Usage

```cpp
GDTPHeader header = receiveHeaderFromNetwork();
std::vector<uint8_t> payload = receivePayloadFromNetwork();
Event event = EventFactory::createEvent(header, payload);
```

This simplifies event creation, delegating it to the **EventFactory**.

### How EventFactory Improves the Event System

By centralizing event creation in **EventFactory**, the code becomes more modular and easier to maintain. When new event types are added, you only need to update the factory, avoiding scattered logic throughout the codebase.

---

## EventPool Class

The **EventPool** class manages the queue of events in a thread-safe manner. It allows multiple threads to add events to the pool and provides mechanisms for retrieving and processing those events.

### Interaction with EventFactory

Instead of manually handling the creation of each event type within **EventPool**, it now delegates that responsibility to **EventFactory**. This decouples event creation logic from the pool, making it easier to extend and maintain.

### Key Methods

#### `void handler(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint)`

This method processes an incoming network message by using **EventFactory** to generate an appropriate **Event**. The generated event is then added to the event queue for further processing.

##### Example Usage

```cpp
GDTPHeader header = receiveHeaderFromNetwork();
std::vector<uint8_t> payload = receivePayloadFromNetwork();
EventPool::getInstance().handler(header, payload, clientEndpoint);
```

This demonstrates how **EventPool** processes incoming data and stores events using **EventFactory**.

#### `Event getNextEvent()`

This method retrieves and removes the next event from the queue. If the queue is empty, an exception is thrown.

```cpp
try {
    Event nextEvent = EventPool::getInstance().getNextEvent();
} catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
}
```

#### `std::vector<Event> getAllEvents()`

Retrieves all events currently in the event queue.

```cpp
std::vector<Event> allEvents = EventPool::getInstance().getAllEvents();
```

#### `void deleteEvent(Event& event)`

Removes a specific event from the queue.

```cpp
Event event = EventPool::getInstance().getNextEvent();
EventPool::getInstance().deleteEvent(event);
```

---

## Event Class

### Overview

The **Event** class represents an event that can occur within the game or network. Each event has a type (represented by **GDTPMessageType**) and a payload that can vary based on the event type. For example, a `PlayerMovement` event contains player movement data, while a `ChatMessage` event holds a message string.

### Constructors

- **Event(EventType type, const PlayerMovement& movement)**: Creates an event with `PlayerMovement` payload.
- **Event(EventType type, const ChatMessage& message)**: Creates an event with `ChatMessage` payload.
- **Event(EventType type, const PlayerShoot& shoot)**: Creates an event with `PlayerShoot` payload.
- **Event(EventType type)**: Creates an event without payload (`NoData`).

#### Example Usage

```cpp
PlayerMovement movement = {1, 10.0f, 20.0f, 30.0f};
Event playerMoveEvent(EventType::PlayerMovement, movement);

ChatMessage message = {1, "Hello!"};
Event chatEvent(EventType::ChatMessage, message);
```

### Key Methods

#### `EventType getType() const`

Returns the type of the event.

#### `const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, NoData>& getPayload() const`

Returns the payload associated with the event. The payload can be one of the following types:
- `PlayerMovement`
- `ChatMessage`
- `PlayerShoot`
- `NoData`

### Payload Types

Each event has an associated payload that stores data specific to the event type. These are the payload types currently implemented:

- **PlayerMovement**: Holds player position data.
- **ChatMessage**: Stores a chat message.
- **PlayerShoot**: Holds information about a player's shooting action.
- **NoData**: Used for events that don't have a payload.

---

## How to Use the Event System

1. **Receiving and Handling Events**  
   When a network message is received, **EventPool** uses **EventFactory** to create the corresponding event:

   ```cpp
   GDTPHeader header = receiveHeaderFromNetwork();
   std::vector<uint8_t> payload = receivePayloadFromNetwork();
   EventPool::getInstance().handler(header, payload, clientEndpoint);
   ```

2. **Retrieving Events**  
   You can retrieve the next event or all events from the **EventPool**:

   ```cpp
   Event nextEvent = EventPool::getInstance().getNextEvent();
   std::vector<Event> allEvents = EventPool::getInstance().getAllEvents();
   ```

3. **Processing Events**  
   After retrieving events, you can process them according to their type:

   ```cpp
   if (nextEvent.getType() == EventType::PlayerMovement) {
       auto movement = std::get<PlayerMovement>(nextEvent.getPayload());
       // Process player movement
   }
   ```

---
