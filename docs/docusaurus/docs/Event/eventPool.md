
---

# EventPool Documentation

## Overview

The `EventPool` class is responsible for managing a queue of `Event` objects, providing functionalities to add, retrieve, and delete events. It serves as a centralized storage and dispatcher for game events, ensuring thread safety through the use of mutexes and condition variables. This class is a singleton, ensuring that only one instance is used throughout the application.

## Table of Contents

1. [Class Overview](#class-overview)
2. [Public Methods](#public-methods)
    - [getInstance()](#getinstance)
    - [addEvent()](#addevent)
    - [getNextEvent()](#getnextevent)
    - [getAllEvents()](#getallevents)
    - [deleteEvent()](#deleteevent)
3. [Private Methods](#private-methods)
    - [handler()](#handler)
4. [Usage Examples](#usage-examples)
5. [Thread Safety](#thread-safety)

---

## Class Overview

```cpp
class EventPool {
public:
    static EventPool& getInstance();
    void addEvent(const Event& event);
    Event getNextEvent();
    std::vector<Event> getAllEvents();
    void deleteEvent(Event& event);

private:
    std::deque<Event> eventQueue; ///< Queue to store events.
    mutable std::mutex eventMutex; ///< Mutex to ensure thread-safety.
    std::condition_variable condition; ///< Condition variable to notify when events are added.
    std::optional<std::function<void(Event)>> _handler;

    void handler(const GDTPHeader &header, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &client_endpoint);
};
```

## Public Methods

### getInstance()

```cpp
static EventPool& getInstance();
```

- **Description**:
  Returns a reference to the singleton instance of `EventPool`. Ensures that only one instance of `EventPool` exists.

- **Returns**:
    - `EventPool&`: A reference to the singleton instance.

- **Example**:

```cpp
// Access the singleton instance of EventPool
EventPool& eventPool = EventPool::getInstance();
```

### addEvent()

```cpp
void addEvent(const Event& event);
```

- **Description**:
  Adds a new `Event` to the `eventQueue`. Notifies any threads waiting for new events.

- **Parameters**:
    - `event`: The `Event` object to be added to the queue.

- **Example**:

```cpp
Event newEvent(EventType::PlayerMovement, playerMovement);
eventPool.addEvent(newEvent);
```

### getNextEvent()

```cpp
Event getNextEvent();
```

- **Description**:
  Retrieves the next `Event` from the `eventQueue`. If the queue is empty, it waits until an event is added.

- **Returns**:
    - `Event`: The next `Event` in the queue.

- **Example**:

```cpp
// Get the next event from the event pool
Event nextEvent = eventPool.getNextEvent();
```

### getAllEvents()

```cpp
std::vector<Event> getAllEvents();
```

- **Description**:
  Retrieves all `Event` objects currently in the `eventQueue` and clears the queue.

- **Returns**:
    - `std::vector<Event>`: A vector containing all events from the queue.

- **Example**:

```cpp
// Retrieve all events from the queue
std::vector<Event> events = eventPool.getAllEvents();
for (const Event& event : events) {
    // Process each event
}
```

### deleteEvent()

```cpp
void deleteEvent(Event& event);
```

- **Description**:
  Removes a specified `Event` from the `eventQueue`. If the event is not found, no action is taken.

- **Parameters**:
    - `event`: A reference to the `Event` object to be deleted.

- **Example**:

```cpp
Event eventToDelete = ...;
eventPool.deleteEvent(eventToDelete);
```

## Private Methods

### handler()

```cpp
void handler(const GDTPHeader &header, const std::vector<uint8_t> &payload, const asio::ip::udp::endpoint &client_endpoint);
```

- **Description**:
  Handles incoming events from the network. Parses the `GDTPHeader` and `payload` to create the appropriate `Event` object and adds it to the `eventQueue`.

- **Parameters**:
    - `header`: The `GDTPHeader` containing metadata about the event.
    - `payload`: The raw binary data of the event.
    - `client_endpoint`: The network endpoint of the client sending the event.

- **Note**:
  This method is intended to be used internally and is not directly accessible outside the class.

## Usage Examples

### Adding and Retrieving Events

```cpp
// Access the singleton instance
EventPool& eventPool = EventPool::getInstance();

// Create an event and add it to the queue
PlayerMovement playerMovement = {1, 100.0f, 200.0f, 0.0f};
Event event(EventType::PlayerMovement, playerMovement);
eventPool.addEvent(event);

// Retrieve the next event from the queue
Event nextEvent = eventPool.getNextEvent();
```

### Handling Multiple Events

```cpp
// Retrieve all events and process them
std::vector<Event> events = eventPool.getAllEvents();
for (const Event& event : events) {
    // Process each event based on its type
    if (event.getType() == EventType::PlayerMovement) {
        // Handle player movement
    } else if (event.getType() == EventType::PlayerShoot) {
        // Handle player shoot
    }
}
```

### Using Custom Event Handlers

```cpp
// Define a custom event handler
auto customHandler = [](const Event& event) {
    std::cout << "Custom event handler called for event type: " << static_cast<int>(event.getType()) << std::endl;
};

// Assign the custom handler to the EventPool
EventPool& eventPool = EventPool::getInstance();
eventPool.setHandler(customHandler);

// Events will now be processed by the custom handler when added
```

## Thread Safety

- The `EventPool` class is designed to be thread-safe, utilizing `std::mutex` for synchronization.
- The `std::condition_variable` ensures that threads waiting for events can be efficiently notified when new events are added.
- This makes `EventPool` suitable for use in multi-threaded environments where different threads may produce and consume events.

---

