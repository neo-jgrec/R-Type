Here’s an updated markdown documentation for both **Event** and **EventPool** classes, with detailed explanations and examples similar to the approach used in the **NetworkingService** documentation.

---

# Event Class Documentation

---

## **Event Class**

The **Event** class represents a wide variety of events that can occur in the system. Each event can have different types of payloads such as player movements, chat messages, or shooting actions. It provides a way to encapsulate the event type and its associated data in a single object.

---

### **1. Overview**

The **Event** class is designed to hold event-related data with different possible payloads, depending on the type of event. The class uses a `std::variant` to store the payload, making it flexible enough to handle events with different types of associated data or no data at all.

---

### **2. Key Features**
- **Type Safety**: The class uses a variant to store payload data for different event types.
- **Multiple Constructors**: Support for constructing events with or without payloads.
- **Encapsulation**: Stores both the event type and its payload in a single object.

---

### **3. Public Methods**

#### **`Event::Event(EventType type, const PlayerMovement& movement)`**
Constructs an event with a `PlayerMovement` payload.

```cpp
PlayerMovement movement{1, 100.0f, 200.0f, 300.0f};
Event event(Event::EventType::PlayerMovement, movement);
```

**Parameters:**
- `type`: The event type.
- `movement`: A `PlayerMovement` struct representing the player's movement.

#### **`Event::Event(EventType type, const ChatMessage& message)`**
Constructs an event with a `ChatMessage` payload.

```cpp
ChatMessage chatMessage{1, "Hello, World!"};
Event event(Event::EventType::ChatMessage, chatMessage);
```

**Parameters:**
- `type`: The event type.
- `message`: A `ChatMessage` struct representing the chat message.

#### **`Event::Event(EventType type, const PlayerShoot& shoot)`**
Constructs an event with a `PlayerShoot` payload.

```cpp
PlayerShoot shoot{1, 2, 1};
Event event(Event::EventType::PlayerShoot, shoot);
```

**Parameters:**
- `type`: The event type.
- `shoot`: A `PlayerShoot` struct representing the player's shooting action.

#### **`Event::Event(EventType type)`**
Constructs an event with no payload.

```cpp
Event event(Event::EventType::GameStart);
```

**Parameters:**
- `type`: The event type.

#### **`EventType Event::getType() const`**
Returns the type of the event.

```cpp
Event::EventType type = event.getType();
```

**Returns:**
- The `EventType` of the event.

#### **`const std::variant<std::monostate, PlayerMovement, ChatMessage, PlayerShoot, NoData>& Event::getPayload() const`**
Returns the payload of the event. Depending on the type of the event, the payload can be of different types (e.g., `PlayerMovement`, `ChatMessage`, etc.).

```cpp
auto payload = event.getPayload();
```

**Returns:**
- A variant containing the event's payload, or `std::monostate` if no data is available.

---

# EventPool Class Documentation

---

## **EventPool Class**

The **EventPool** class provides a thread-safe pool for storing and managing game or network events. It allows multiple threads to push and pop events while ensuring synchronization.

---

### **1. Overview**

The **EventPool** class acts as a central repository for events in a multithreaded environment. Events can be added to the pool from different threads, and consumers can pop or retrieve events from the pool for processing.

---

### **2. Key Features**
- **Thread-Safety**: Uses mutexes and condition variables to ensure that event operations are synchronized.
- **Event Handling**: Supports custom event handlers for processing received events.
- **Queue-Based Storage**: Uses a deque to store and retrieve events in a FIFO manner.

---

### **3. Public Methods**

#### **`Event EventPool::getNextEvent()`**
Gets the next event from the queue and removes it.

```cpp
Event nextEvent = EventPool::getInstance().getNextEvent();
```

**Returns:**
- The next event in the queue.
- Throws a `std::runtime_error` if the queue is empty.

**Example Usage:**
```cpp
try {
    Event event = EventPool::getInstance().getNextEvent();
    // Process event...
} catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
}
```

#### **`std::vector<Event> EventPool::getAllEvents()`**
Retrieves all events from the queue without removing them.

```cpp
std::vector<Event> allEvents = EventPool::getInstance().getAllEvents();
```

**Returns:**
- A `std::vector` containing all events currently in the queue.

#### **`void EventPool::deleteEvent(Event& event)`**
Deletes a specific event from the pool.

```cpp
EventPool::getInstance().deleteEvent(event);
```

**Parameters:**
- `event`: The event to be deleted.

#### **`void EventPool::pushEvent(const Event& event)`**
Adds an event to the pool. This method is thread-safe and notifies waiting threads that an event has been added.

```cpp
EventPool::getInstance().pushEvent(event);
```

**Parameters:**
- `event`: The event to be added to the pool.

#### **`std::optional<Event> EventPool::popEvent()`**
Retrieves and removes the next event from the pool. If the pool is empty, it returns an empty optional.

```cpp
std::optional<Event> eventOpt = EventPool::getInstance().popEvent();
```

**Returns:**
- The next event in the pool or an empty optional if no events are available.

#### **`bool EventPool::isEmpty() const`**
Checks if the event pool is empty.

```cpp
bool isEmpty = EventPool::getInstance().isEmpty();
```

**Returns:**
- `true` if the pool is empty, `false` otherwise.

---

### **4. Private Methods (Event Handlers)**

#### **`void EventPool::handlePlayerMovement(const GDTPHeader &header, const std::vector<uint8_t> &payload)`**
Handles a `PlayerMovement` event by converting the payload and pushing the event into the queue.

#### **`void EventPool::handlePlayerShoot(const GDTPHeader &header, const std::vector<uint8_t> &payload)`**
Handles a `PlayerShoot` event by converting the payload and pushing the event into the queue.

#### **`void EventPool::handleChatMessage(const GDTPHeader &header, const std::vector<uint8_t> &payload)`**
Handles a `ChatMessage` event by converting the payload and pushing the event into the queue.

---

### **5. Example Usage**

```cpp
// Pushing a new event into the pool
PlayerMovement movement{1, 100.0f, 200.0f, 300.0f};
Event event(Event::EventType::PlayerMovement, movement);
EventPool::getInstance().pushEvent(event);

// Retrieving the next event
Event nextEvent = EventPool::getInstance().getNextEvent();
```

---

### **6. Error Handling**

#### **UnknownEvent Exception**
Thrown when an unrecognized event type is encountered.

```cpp
throw EventPool::UnknownEvent(header.messageType);
```

---

This markdown file documents both classes in detail, with explanations and practical code examples to guide developers in using these classes effectively.