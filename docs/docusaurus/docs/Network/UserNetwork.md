---
title: Network User Documentation
sidebar_position: 2

---

## **1. Overview**

The **NetworkingService** class provides a simple and efficient way to handle **UDP-based communication** using the **Game Datagram Transfer Protocol (GDTP)**. This service is essential for exchanging real-time game data between a client and server, such as player movements, entity updates, and game events.

This guide will walk you through the steps to:
1. **Initialize the service**.
2. **Send requests** to clients or servers.
3. **Receive and handle messages**.
4. **Manage errors and payload preparation**.

---

## **2. Getting Started**

### **2.1. Initializing the Service**

The **NetworkingService** is implemented as a **singleton**, ensuring there is only one instance of the service throughout your application. The first time you call the `getInstance` method, it initializes the service with the given IP address and port. Subsequent calls will return the same instance.

```cpp
// Initialize the NetworkingService with server IP and port
NetworkingService& networkService = NetworkingService::getInstance("127.0.0.1", 8080);

// Or you can use the default parameters
NetworkingService& defaultService = NetworkingService::getInstance();
```

This setup starts listening for incoming UDP packets as soon as the service is initialized.

---

## **3. Sending Requests**

The **sendRequest** method is the primary way to send data using the GDTP protocol. You can send requests either by specifying the recipient's IP address and port or by using an **ASIO endpoint**.

### **3.1. Sending a Request by IP and Port**

```cpp
// Sending a request to a specific IP and port
networkService.sendRequest("127.0.0.1", 12345, uint8_t::PlayerMovement, {"1", "100.0", "200.0", "300.0"});
```

In this example:
- **"127.0.0.1"** is the IP address of the recipient.
- **12345** is the port.
- **`PlayerMovement`** is the message type (e.g., for player movement).
- The payload consists of a **Player ID** followed by **X, Y, and Z coordinates**.

### **3.2. Sending a Request by Endpoint**

You can also send requests using an **ASIO UDP endpoint**.

```cpp
asio::ip::udp::endpoint clientEndpoint(asio::ip::address::from_string("127.0.0.1"), 8080);
networkService.sendRequest(clientEndpoint, uint8_t::PlayerMovement, {"1", "100.0", "200.0", "300.0"});
```

This method uses the **clientEndpoint** as the destination for the request.

---

## **4. Receiving and Handling Messages**

The service automatically listens for incoming packets once initialized. To process different **GDTP message types**, you must define **message handlers**.

### **4.1. Setting a Message Handler**

Each message type can have a handler function that processes the received data. You can set a message handler like this:

```cpp
networkService.setMessageHandler(uint8_t::PlayerMovement, 
    [](const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint) {
        // Example: Handle player movement
        std::cout << "Received PlayerMovement message!" << std::endl;
        // Process the payload, e.g., extracting player position data
    }
);
```

**Parameters:**
- **header**: Contains metadata (version, message type, packet ID, etc.).
- **payload**: The actual game data (e.g., player ID and coordinates).
- **client_endpoint**: The sender's IP and port.

### **4.2. Receiving Messages**

The **NetworkingService** continuously listens for incoming messages in the background. When a message is received, the appropriate handler is automatically invoked, based on the **message type**.

---

## **5. Payload Preparation**

Before sending data, you may need to convert user inputs (such as strings or numbers) into a binary format suitable for transmission. The **NetworkingService** allows you to define custom payload handlers for each message type.

### **5.1. Setting a Payload Handler**

Define a payload handler for a specific message type, like **PlayerMovement**:

```cpp
networkService.setPayloadHandler(uint8_t::PlayerMovement, 
    [](std::vector<std::string> args) -> std::vector<uint8_t> {
        // Convert the args into a binary format
        std::vector<uint8_t> payload;
        // For example, convert player ID and coordinates to binary
        return payload;
    }
);
```

**Parameters:**
- **args**: A vector of string arguments (e.g., player ID, coordinates).
- **Returns**: A binary payload (`std::vector<uint8_t>`) that will be sent with the request.

---

## **6. Error Handling**

The **NetworkingService** class includes error handling to manage cases where no payload handler is defined for a given message type.

### **6.1. UnknownPayloadMessage Exception**

If you attempt to send a message without a defined payload handler, an **UnknownPayloadMessage** exception is thrown.

```cpp
if (!payload_handlers->contains(messageType)) {
    throw UnknownPayloadMessage(messageType);
}
```

This ensures that you are alerted if a message type is missing its payload preparation logic.

---

## **7. Handling Specific Message Types**

Below are a few examples of common GDTP message types and how to handle them:

### **7.1. Player Movement Message (0x01)**

This message updates the player's position. The payload includes the **Player ID** and **X, Y, Z coordinates**.

**Payload Structure:**
- Player ID (4 bytes)
- X, Y, Z coordinates (each 4 bytes, float)

**Example Handler:**
```cpp
networkService.setMessageHandler(uint8_t::PlayerMovement, 
    [](const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint) {
        uint32_t playerId;
        float x, y, z;
        
        // Extract the player ID and coordinates from the payload
        std::memcpy(&playerId, &payload[0], 4);
        std::memcpy(&x, &payload[4], 4);
        std::memcpy(&y, &payload[8], 4);
        std::memcpy(&z, &payload[12], 4);

        std::cout << "Player " << playerId << " moved to position (" << x << ", " << y << ", " << z << ")" << std::endl;
    }
);
```

---

## **8. Best Practices**

### **8.1. Managing Packet Loss**
GDTP over UDP does not guarantee reliable delivery of packets. Therefore, the application logic should tolerate missing packets or handle retransmission when necessary.

- **Acknowledgment Packets (0x04)**: These can be used to confirm the receipt of critical packets.
- **Sequence Numbers**: These help reorder packets if they arrive out of sequence.

### **8.2. Keep Payloads Small**
Since UDP has a limited packet size (typically 1400 bytes), ensure that your payloads fit within these constraints. Larger data should be split across multiple packets.

---

## **9. Example Usage**

Here’s a simple example of how you might set up the **NetworkingService** and use it to handle player movement messages:

```cpp
int main() {
    // Initialize the network service
    NetworkingService& networkService = NetworkingService::getInstance("127.0.0.1", 8080);
    
    // Set up a handler for player movement messages
    networkService.setMessageHandler(uint8_t::PlayerMovement, 
        [](const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint) {
            // Handle player movement
            uint32_t playerId;
            float x, y, z;
            std::memcpy(&playerId, &payload[0], 4);
            std::memcpy(&x, &payload[4], 4);
            std::memcpy(&y, &payload[8], 4);
            std::memcpy(&z, &payload[12], 4);
            std::cout << "Player " << playerId << " moved to position (" << x << ", " << y << ", " << z << ")" << std::endl;
        }
    );

    // Run the network service in a separate thread
    networkService.run();

    // Main game loop here
    // ...

    return 0;
}
```

---

## **10. Conclusion**

The **NetworkingService** provides an easy-to-use API for handling **UDP communication** in real-time games, including support for **GDTP message types**, payload preparation, and error management. By following the examples and best practices outlined in this documentation, developers can efficiently manage client-server communication for fast-paced, low-latency game environments.

---