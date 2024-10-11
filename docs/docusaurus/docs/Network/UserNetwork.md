### NetworkingService - User Documentation

---

## Overview

The `NetworkingService` is a tool that helps you manage real-time communication between clients and servers using a custom protocol called GDTP (Game Datagram Transfer Protocol). This tool is designed to keep the communication fast and smooth, making it ideal for multiplayer games where every millisecond counts.

With the `NetworkingService`, you can:
- Send and receive messages between the game server and players.
- Process different types of game events like player movements, shooting, and more.
- Use a simple setup to get everything running in no time.

## Key Features

- **Fast Communication**: Uses UDP for sending and receiving data quickly, making sure your game doesn't lag.
- **Custom Handlers**: Allows you to define how different types of messages are handled.
- **Single Instance**: Manages all communication from a single point, making it easy to use throughout your game.

---

## Getting Started

### Step 1: Setting Up the Service

The `NetworkingService` is designed to ensure you only need one instance of it. You can create this instance and start it with the following code:

```cpp
auto& networkService = NetworkingService::getInstance("127.0.0.1", 12345);
networkService.run();  // Starts the service in a background thread.
```

In this example:
- `"127.0.0.1"` is the IP address where your service will be listening.
- `12345` is the port number where the service will send and receive data.

### Step 2: Listening for Messages

After setting up, you can start receiving messages from other players or the server:

```cpp
networkService.startReceive();
```

This makes sure that the service is ready to listen for any incoming messages, like player actions or game updates.

---

## Sending Messages

You can send different types of messages to clients or the server, like player movements, shooting actions, or game events. Here’s how you do it:

### Example: Sending a Player Movement

```cpp
std::vector<uint8_t> payload = {0x01, 0x02, 0x03}; // Example movement data
networkService.sendRequest("127.0.0.1", 12345, 0x01, payload);
```

In this example:
- `"127.0.0.1"` is the recipient's IP address.
- `12345` is the recipient's port.
- `0x01` is the message type, indicating this message is about player movement.
- `payload` contains the actual data, like coordinates.

### Example: Sending a Response

If you need to respond to a message you received, you can use:

```cpp
GDTPHeader header = ...; // This would be the header of the message you received
std::vector<uint8_t> responsePayload = {0xAA, 0xBB}; // Data you want to send back
networkService.sendRequestResponse("127.0.0.1", 12345, header, responsePayload);
```

This uses the same message header that was received, making it easy to respond to specific actions or events.

---

## Handling Different Game Events

One of the best features of `NetworkingService` is that you can define how different game events should be processed. For example, you might want to take some action when a player shoots or when their position changes.

### Example: Handling a Player Shoot

```cpp
networkService.addEvent(0x05, [](const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client) {
    // Your logic for handling a player shoot goes here
    std::cout << "Player shot detected from: " << client.address() << std::endl;
});
```

In this example:
- `0x05` represents the "Player Shoot" event.
- The lambda function specifies what to do when this event occurs, like updating the game state or notifying other players.

---

## Stopping the Service

When you're done with the service (for example, when shutting down the server), you should stop it gracefully:

```cpp
networkService.stop();
networkService.attempt(); // Ensures the network thread has finished.
```

This ensures that the service stops listening for messages and that all ongoing communication is properly terminated.

---

## Best Practices

1. **Start the Service Early**: It’s a good idea to start the `NetworkingService` as soon as your game or server starts, so it’s ready to handle messages immediately.

2. **Use Events to Handle Different Actions**: Register different events to handle specific actions like player movements or shooting. This helps keep your game logic organized.

3. **Graceful Shutdown**: Always call `stop()` before exiting your game or server to make sure that all connections are properly closed.

---

## Summary

The `NetworkingService` makes managing network communication for multiplayer games simple and efficient. Whether you’re sending game state updates, responding to player actions, or managing server-client communication, this service helps you handle it all seamlessly. By following the examples and best practices, you can ensure smooth and responsive gameplay for all players.