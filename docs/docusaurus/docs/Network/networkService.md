# NetworkingService Developer Documentation

## Overview
The `NetworkingService` class manages real-time communication between clients and servers using the Game Datagram Transfer Protocol (GDTP) over UDP. It supports sending and receiving game-related data such as player movements, entity updates, and events. This class is designed to ensure low-latency communication, making it suitable for real-time multiplayer game environments.

## Class: `NetworkingService`

### Key Characteristics:
- **Transport Layer**: Uses UDP for low-latency, high-speed communications.
- **Asynchronous I/O**: Leverages ASIO for non-blocking network operations.
- **Event-Driven Architecture**: Uses handlers for processing specific message types.
- **Singleton Pattern**: Ensures a single instance of the service for consistent network behavior across the application.

### Protocol Overview:
The GDTP protocol uses a fixed header structure:
- **Version (1 byte)**: Protocol version (currently `0x01`).
- **Message Type (1 byte)**: Identifies the type of message.
- **Packet ID (8 bytes)**: Unique identifier for the packet.
- **Payload Size (2 bytes)**: The size of the message payload.
- **Sequence Number (2 bytes)**: Order of the packet in a sequence.
- **Total Packets (2 bytes)**: Indicates how many packets make up the full message.

### Public Methods

#### `static NetworkingService& getInstance(const std::string& server_ip = "127.0.0.1", int port = 12345)`
- **Description**: Retrieves the singleton instance of `NetworkingService`, creating it if it doesn't exist.
- **Parameters**:
    - `server_ip` (const std::string&): IP address of the server.
    - `port` (int): The port number for communication.
- **Example**:
  ```cpp
  NetworkingService& networkService = NetworkingService::getInstance("192.168.1.10", 8080);
  networkService.run();  // Start the service.
  ```

#### `void addEvent(const uint8_t messageType, const std::function<void(const GDTPHeader&, const std::vector<uint8_t>&, const asio::ip::udp::endpoint&)>& handler)`
- **Description**: Registers a handler function for a specific GDTP message type. When a message of this type is received, the registered function is called.
- **Parameters**:
    - `messageType` (uint8_t): The type of message for which the handler is being registered.
    - `handler`: A function that processes the message's header, payload, and client endpoint.
- **Example**:
  ```cpp
  networkService.addEvent(0x01, [](const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client) {
      std::cout << "Player movement received from " << client.address() << std::endl;
  });
  ```

#### `std::string getLocalEndpoint() const`
- **Description**: Retrieves the local endpoint (IP:Port) where the service is listening.
- **Returns**: A string representation of the local endpoint.
- **Example**:
  ```cpp
  std::string endpoint = networkService.getLocalEndpoint();
  std::cout << "Listening on: " << endpoint << std::endl;
  ```

#### `std::string getIp() const`
- **Description**: Retrieves the local IP address where the service is listening.
- **Returns**: The IP address as a string.
- **Example**:
  ```cpp
  std::string ip = networkService.getIp();
  std::cout << "Local IP: " << ip << std::endl;
  ```

#### `uint32_t getPort() const`
- **Description**: Retrieves the local port number where the service is listening.
- **Returns**: The port number.
- **Example**:
  ```cpp
  uint32_t port = networkService.getPort();
  std::cout << "Listening on port: " << port << std::endl;
  ```

### Sending and Receiving Messages

#### `void sendRequest(const std::string& recipient, int port, uint8_t messageType, const std::vector<uint8_t>& payload = {})`
- **Description**: Sends a message with a specified type and payload to a recipient.
- **Parameters**:
    - `recipient`: IP address of the recipient.
    - `port`: Port number on which to send the message.
    - `messageType`: Type of message (e.g., `0x01` for PlayerMovement).
    - `payload`: Data to be included in the message.
- **Example**:
  ```cpp
  std::vector<uint8_t> data = {0x01, 0x02, 0x03};
  networkService.sendRequest("127.0.0.1", 12345, 0x01, data);
  ```

#### `void sendRequest(const asio::ip::udp::endpoint& client_endpoint, uint8_t messageType, const std::vector<uint8_t>& payload = {})`
- **Description**: Sends a message directly to a specified client endpoint.
- **Example**:
  ```cpp
  asio::ip::udp::endpoint client(asio::ip::make_address("127.0.0.1"), 12345);
  networkService.sendRequest(client, 0x01, {0x10, 0x20});
  ```

#### `void sendRequestResponse(const std::string& recipient, int port, GDTPHeader headerOrigin, const std::vector<uint8_t>& payload = {})`
- **Description**: Sends a response using a given header and payload.
- **Parameters**:
    - `headerOrigin`: Original header used as the basis for the response.
- **Example**:
  ```cpp
  GDTPHeader header = ...; // Received from another message
  networkService.sendRequestResponse("127.0.0.1", 12345, header, {0xAA, 0xBB});
  ```

### Internal Methods

#### `void startReceive()`
- **Description**: Starts listening for incoming packets asynchronously.
- **Example**:
  ```cpp
  networkService.startReceive(); // Begin listening for messages.
  ```

#### `void handleReceivedPacket(const std::array<uint8_t, 1400>& packet, std::size_t length, const asio::ip::udp::endpoint& client_endpoint) const`
- **Description**: Validates and processes an incoming packet.
- **Example**:
  ```cpp
  handleReceivedPacket(buffer, length, client_endpoint);
  ```

#### `void processMessage(uint8_t messageType, const std::vector<uint8_t>& payload, const GDTPHeader& header, const asio::ip::udp::endpoint& client_endpoint) const`
- **Description**: Processes a message by invoking the appropriate handler.
- **Example**:
  ```cpp
  processMessage(0x01, payload, header, client_endpoint);
  ```

---

## Best Practices for Developers

1. **Starting the Service**:
    - Call `getInstance()` to create or retrieve the singleton instance.
    - Use `startReceive()` followed by `run()` to start receiving messages.
    - Example:
      ```cpp
      auto& service = NetworkingService::getInstance();
      service.startReceive();
      service.run();
      ```

2. **Adding Event Handlers**:
    - Use `addEvent()` to register handlers for different message types.
    - Ensure each handler processes the payload correctly according to its type.
    - Example:
      ```cpp
      service.addEvent(0x05, [](const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client) {
          std::cout << "Handling a Player Shoot event." << std::endl;
      });
      ```

3. **Shutting Down the Service**:
    - Call `stop()` before the application exits to gracefully close network operations.
    - Use `attempt()` to wait until the network thread is fully stopped.
    - Example:
      ```cpp
      service.stop();
      service.attempt();
      ```

4. **Sending Requests and Responses**:
    - Use `sendRequest()` to send messages to clients or servers.
    - Use `sendRequestResponse()` to reply to received messages, preserving header metadata.

---

