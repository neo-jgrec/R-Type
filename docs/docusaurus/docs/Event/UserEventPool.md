
---

# EventPool - User Guide

## What is EventPool?

The `EventPool` is like a message box for events in the game. Think of it as a place where events (such as a player moving, shooting, or an item being picked up) are stored until they are needed. It helps manage these events so that they can be processed in the right order and at the right time.

## Why Use EventPool?

Imagine a game where multiple things are happening all at once: players move, enemies appear, and power-ups are collected. It’s important to keep track of these events and handle them one by one. The `EventPool` makes this easier by organizing the events in a queue (a kind of list) and allowing you to add new events or get the next event when you’re ready to process it.

## How to Use EventPool

### Getting Started

The `EventPool` is designed to be used as a "singleton." This means that there is only one `EventPool` in the game, and everyone uses the same one. To get this instance, you use the `getInstance` function:

```cpp
// Get access to the EventPool
EventPool& eventPool = EventPool::getInstance();
```

Now, you have access to the `EventPool` and can start adding or retrieving events.

### Adding an Event

To add a new event to the `EventPool`, you use the `addEvent` function. Let’s say a player has moved in the game:

```cpp
// Create a PlayerMovement event
PlayerMovement movement = {1, 100.0f, 200.0f, 0.0f}; // Player ID 1 moves to (100, 200, 0)
Event movementEvent(0x01, movement); // 0x01 is the event type for PlayerMovement

// Add this event to the pool
eventPool.addEvent(movementEvent);
```

This will put the movement event in the queue, ready to be processed later.

### Getting the Next Event

Once you have added events to the `EventPool`, you might want to retrieve the next one to see what needs to be handled:

```cpp
// Get the next event in the queue
Event nextEvent = eventPool.getNextEvent();
```

This will give you the next event from the queue and remove it, so you can handle it in your game.

### Getting All Events at Once

If you want to process all the events that have been stored in the `EventPool`, you can get all of them at once:

```cpp
// Retrieve and process all events
std::vector<Event> allEvents = eventPool.getAllEvents();
for (const Event& event : allEvents) {
    // Process each event
}
```

This is useful if you want to handle all the events in one go, instead of processing them one by one.

### Deleting an Event

If you want to remove a specific event from the `EventPool`, you can use the `deleteEvent` function:

```cpp
// Suppose we have an event that we want to remove
eventPool.deleteEvent(movementEvent);
```

This will find and remove the `movementEvent` from the queue if it’s there.

## Example - Using EventPool in a Game Loop

Here’s an example of how you might use the `EventPool` in a game loop to handle events like player actions:

```cpp
// Get the EventPool instance
EventPool& eventPool = EventPool::getInstance();

// Game loop
while (isGameRunning) {
    // Add a new event (e.g., player movement)
    PlayerMovement move = {1, 150.0f, 250.0f, 0.0f};
    Event event(0x01, move);
    eventPool.addEvent(event);

    // Process all events
    std::vector<Event> events = eventPool.getAllEvents();
    for (const Event& e : events) {
        if (e.getType() == 0x01) {
            // Handle player movement
        } else if (e.getType() == 0x05) {
            // Handle player shoot
        }
        // Add more cases for other types of events...
    }
}
```

## Summary

The `EventPool` makes it easy to keep track of all the actions happening in the game. You can add new events when things happen (like a player moving or an enemy appearing), and then process them later when you’re ready. It’s like a to-do list for game events!

With this simple guide, you can start using `EventPool` in your game to manage events smoothly and keep your game logic organized.

---

