---
title: NetworkingService Class Documentation
sidebar_position: 1
---
1. **Overview of GDTP** and how **UDP-based communication** is integrated.
2. **Explanation of key components**: message handling, payload preparation, and error management.
3. **Details of each public method** with code examples where necessary.

### NetworkingService Class Documentation

---

## **NetworkingService Class**
This class provides UDP-based networking services, enabling communication between a client and server using the **Game Datagram Transfer Protocol (GDTP)**. It handles message sending, reception, and processing, using ASIO for asynchronous network operations. The class also employs the **Singleton Design Pattern** to ensure only one instance of the networking service exists throughout the application.

---

### **1. Class Overview**

**NetworkingService** allows the exchange of real-time game data such as player movements, entity updates, and in-game events between the client and server using **GDTP over UDP**. The class includes mechanisms to handle packet loss, reordering, and message validation, which are critical for reliable and low-latency game communication.

#### **Key Features:**
- **UDP Protocol**: Used for fast, connectionless communication with low overhead.
- **Singleton Design Pattern**: Ensures a single instance of the class is shared across the application.
- **Message Handlers**: Processes messages based on their type.
- **Payload Preparation Handlers**: Converts input arguments into a binary payload before sending.

---

### **2. GDTP Protocol Integration**

The **Game Datagram Transfer Protocol (GDTP)** is designed for lightweight, low-latency communication in real-time games. This protocol leverages **UDP** to send discrete datagrams between a client and server.

Each datagram consists of:
- **Header (16 bytes)**: Contains version, message type, packet ID, sequence number, total packets, and payload size.
- **Payload (variable size)**: Carries data specific to the message type.

#### **UDP-Specific Concerns:**
- **Packet Loss**: Handled by the game logic, which allows updates from subsequent packets.
- **Out-of-Order Packets**: Sequence numbers help reorder packets upon receipt.
- **Fixed-Size Buffers**: Protect against buffer overflow attacks by discarding oversized packets.

---

### **3. Public Methods**

#### **`getInstance`**
This static method returns the singleton instance of **NetworkingService**. It ensures only one instance exists across the application, which is initialized with the provided IP address and port on the first call.

```cpp
NetworkingService& networkService = NetworkingService::getInstance("192.168.1.1", 8080);
```

**Parameters:**
- `server_ip` (optional): IP address of the server (default: `"127.0.0.1"`).
- `port` (optional): Port number for the server (default: `12345`).

**Returns:**
- A reference to the singleton **NetworkingService** instance.

---

#### **`setMessageHandler`**
This method assigns a handler function to a specific **GDTP message type**. When a message of this type is received, the handler processes the message's header, payload, and client endpoint.

```cpp
networkService.setMessageHandler(uint8_t::PlayerMovement, 
    [](const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint) {
        std::cout << "Player moved!" << std::endl;
});
```

**Parameters:**
- `messageType`: The GDTP message type (e.g., **PlayerMovement**).
- `handler`: A function that processes the message (header, payload, client endpoint).

---

#### **`setPayloadHandler`**
This method assigns a function to prepare the payload for a specific **GDTP message type**. The handler converts input arguments into a binary payload that can be sent.

```cpp
networkService.setPayloadHandler(uint8_t::PlayerMovement, 
    [](std::vector<std::string> args) -> std::vector<uint8_t> {
        // Prepare payload from arguments
        return std::vector<uint8_t>();
});
```

**Parameters:**
- `messageType`: The GDTP message type (e.g., **PlayerMovement**).
- `handler`: A function that prepares the payload.

---

#### **`sendRequest`**
This method sends a request to a specified recipient, composed of a GDTP message type, header, and payload. It supports both IP address and **ASIO endpoint** as recipients.

```cpp
networkService.sendRequest("127.0.0.1", 12345, uint8_t::PlayerMovement, {"1", "100.0", "200.0", "300.0"});
```

```cpp
asio::ip::udp::endpoint clientEndpoint(asio::ip::address::from_string("127.0.0.1"), 8080);
networkService.sendRequest(clientEndpoint, uint8_t::PlayerMovement, {"1", "100.0", "200.0", "300.0"});
```

**Parameters:**
- `recipient`: The recipient's IP address or UDP endpoint.
- `port`: Port number of the recipient.
- `messageType`: The GDTP message type.
- `args`: Arguments for the payload (optional).

---

#### **`startReceive`**
This method begins the asynchronous reception of UDP packets. It continuously listens for incoming packets using ASIO’s **`async_receive_from`** and processes them with the assigned handlers.

---

#### **`run`**
Starts the ASIO I/O context in a separate thread, enabling asynchronous I/O operations.

---

#### **`stop`**
Stops the ASIO I/O context and terminates the service thread.

---

### **4. Error Handling**

#### **UnknownPayloadMessage**
This exception is thrown when a payload handler for a specific **GDTP message type** is missing.

```cpp
throw UnknownPayloadMessage(messageType);
```

**Example Usage:**
```cpp
if (!payload_handlers->contains(messageType)) {
    throw UnknownPayloadMessage(messageType);
}
```

---

### **5. Message Handling**

Message handling is a core aspect of the **NetworkingService** class. Each message type has its own handler, responsible for processing incoming data.

When a packet is received, the **header** and **payload** are extracted and passed to the appropriate handler function based on the **messageType**.

---

### **6. GDTP Packet Structure**

Each packet transmitted or received follows the GDTP format:

| Field           | Size (bytes) |
|-----------------|--------------|
| Version         | 1            |
| Message Type    | 1            |
| Packet ID       | 8            |
| Payload Size    | 2            |
| Sequence Number | 2            |
| Total Packets   | 2            |

- **Payload (variable size)**: The content of the message depends on the **message type**.

Example packet (Player Movement):
```cpp
| Version | MessageType | PacketID | PayloadSize | SeqNum | TotalPackets |
|    1    |      1      |    1     |      8      |    1   |       1      |
|  Player ID | X coordinate | Y coordinate | Z coordinate |
```

---

### **7. Conclusion**

The **NetworkingService** class provides robust and flexible UDP-based communication for real-time games, integrating smoothly with the **GDTP protocol**. The class is designed to handle common networking concerns like packet loss, ordering, and payload preparation, making it a foundational component for client-server communication in your project.

---