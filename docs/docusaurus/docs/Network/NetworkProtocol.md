# Game Datagram Transfer Protocol (GDTP) — Binary Protocol for Client/Server Communications Using UDP

## Protocol Overview

The **Game Datagram Transfer Protocol (GDTP)** is a lightweight, low-latency binary protocol designed for real-time game communications over **UDP**. The protocol is optimized for the efficient transmission of game data such as player movement, entity updates, and events between clients and a server. Given **UDP**’s connectionless and unreliable nature, this protocol includes mechanisms to handle packet loss, packet ordering, and message validation without introducing significant overhead.

### Key Characteristics

- **Transport Layer**: **UDP** for all in-game communications.
- **Secondary TCP Usage**: Used for non-time-sensitive tasks such as authentication, initial connection setup, or large data transfers (e.g., downloading maps or assets). This is justified by **TCP**’s reliability, which is important for these tasks, but not needed for fast-paced gameplay.
- **Packet-Oriented Communication**: Every message sent between a client and server is a discrete datagram with its own header and payload, designed to be small enough to fit within typical **UDP** packet limits (~1400 bytes).
- **Error Handling and Security**: Malformed packets are discarded without crashing the server or client. Buffer overflow and memory safety are ensured by fixed-size buffers, validation checks, and error codes.

---
## Protocol Design

### Packet Structure

Each packet in GDTP consists of two main parts:

- **Header (16 bytes)**
- **Payload (variable size, depends on message type)**

### Header (16 bytes):

| Field               | Size     | Description                                                                 |
|---------------------|----------|-----------------------------------------------------------------------------|
| **Version**         | 1 byte   | The protocol version (e.g., `0x01`).                                         |
| **Message Type**    | 1 byte   | Identifies the purpose of the message (e.g., `0x01` for player movement).    |
| **Packet ID**       | 8 bytes  | A unique identifier for this packet (useful for ensuring proper sequencing). |
| **Payload Size**    | 2 bytes  | Size of the payload, in bytes.                                               |
| **Sequence Number** | 2 bytes  | Tracks the ordering of packets.                                              |
| **Total Packets**   | 2 bytes  | Indicates the total number of packets in a series.                           |

### Payload (Variable):

The payload size and structure depend on the **Message Type**. Different message types can carry different data.

---

### Error Handling

1. **Malformed Packets**:
   - If a packet arrives that does not conform to the expected format, it is discarded. The server/client does not crash or allocate additional memory for malformed packets.
   - *Checksum validation* ensures that corrupted packets are discarded early without processing.

2. **Buffer Overflow Protection**:
   - The server/client will maintain fixed-size buffers, typically sized for the maximum expected packet (1400 bytes for a UDP datagram). If a packet exceeds the expected size (based on the *Payload Size* field), it is discarded.

3. **Handling Packet Loss and Reordering**:
   - The game logic is designed to be *state-based*, meaning missing packets can be handled gracefully by allowing updates from subsequent packets.

4. **Duplicate Packets**:
   - The *Packet ID* field ensures that duplicate packets can be identified and ignored.


---

### Protocol Documentation

The GDTP is designed with simplicity in mind, focusing on fast, reliable game communication while maintaining a small memory footprint. Below are specific documentation aspects:

**Message Definition:**

Each message type in GDTP has a defined structure. The exact byte layout must be respected to ensure compatibility between clients and servers.

**Example: Player Movement Packet**

```Markdown
Header:
| Version | Message Type | Packet ID | Payload Size | Seq Number | Total Packet |
|   1     |      1       |    8      |      2       |     2      |      2       |    
Payload:
|  Player ID | X coordinate | Y coordinate | Z coordinate |
|     4      |       4      |      4       |      4       |
```

### Integrity and Security:

- **Checksum**: Every packet includes a simple checksum in its header to ensure that the message has not been corrupted. The checksum is calculated by XOR-ing all bytes in the packet and verifying this at the receiver side.

- **Fixed-size Buffers**: Both the client and server use fixed-size buffers for receiving packets. If a packet exceeds the size defined by the `Payload Size` field, it is discarded to prevent buffer overflows.

### UDP-Specific Concerns:

1. **Handling Packet Loss**: The server can tolerate missing movement or event packets due to the state-based nature of the game. If the client sends regular updates, missing one or two packets doesn’t drastically affect the game state.

2. **Out-of-order Packets**: Each packet includes a `sequence number` to detect and reorder out-of-sequence packets.

---

## Example Implementation
### Server Code (UDP-based using `boost::asio`)

```cpp
#include <boost/asio.hpp>
#include <iostream>
#include <vector>

using boost::asio::ip::udp;

class GameServer {
public:
    GameServer(boost::asio::io_context& io_context, short port)
        : socket_(io_context, udp::endpoint(udp::v4(), port)) {
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    handle_packet(bytes_recvd);
                }
                start_receive();
            });
    }
    void handle_packet(std::size_t length) {
        if (length < 10) return;  // Minimum length for header
        uint8_t version = recv_buffer_[0];
        uint8_t message_type = recv_buffer_[1];
        if (version != 0x01) return;  // Unsupported version
        uint16_t payload_size = (recv_buffer_[6] << 8) | recv_buffer_[7];
        if (length != 10 + payload_size) return;  // Incorrect payload size
        // Example: Handle Player Movement
        if (message_type == 0x01 && payload_size == 12) {
            uint32_t player_id;
            float x, y, z;
            std::memcpy(&player_id, &recv_buffer_[10], 4);
            std::memcpy(&x, &recv_buffer_[14], 4);
            std::memcpy(&y, &recv_buffer_[18], 4);
            std::memcpy(&z, &recv_buffer_[22], 4);
            std::cout << "Player " << player_id << " moved to (" << x << ", " << y << ", " << z << ")\n";
        }
    }
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<uint8_t, 1400> recv_buffer_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        GameServer server(io_context, 12345);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}
```

---
## Message Types

### 1. Messages related to Players:

- **Player Movement (`0x01`)**: Updates the position of a player, including player ID and 3D coordinates.

  **Payload**:

    - Player ID (4 bytes)
    - X coordinate (4 bytes, float)
    - Y coordinate (4 bytes, float)
    - Z coordinate (4 bytes, float)

- **Player Shoot (`0x05`)**: Indicates that a player has fired a missile or weapon.

  **Payload**:

    - Player ID (4 bytes)
    - Direction (1 byte)
    - Weapon Type (1 byte)

- **Player Health Update (`0x06`)**: Updates the health of a player after being hit.

  **Payload**:

    - Player ID (4 bytes)
    - Health (1 byte)

- **Player Respawn (`0x07`)**: Notification that a player has respawned after dying.

  **Payload**:

    - Player ID (4 bytes)
    - X coordinate (4 bytes)
    - Y coordinate (4 bytes)
    - Z coordinate (4 bytes)

- **Player Disconnect (`0x08`)**: A player has quit or disconnected.

  **Payload**:

    - Player ID (4 bytes)

### 2. Messages related to Entities and the Game:

- **Entity Update (`0x03`)**: Updates the information of an entity (e.g., enemies, objects, projectiles).

  **Payload**:

    - Entity ID (4 bytes)
    - Entity Type (2 bytes)
    - X, Y, Z coordinates (12 bytes, floats)

- **Entity Spawn (`0x09`)**: Creates a new entity in the game (e.g., monster, power-up).

  **Payload**:

    - Entity ID (4 bytes)
    - Entity Type (2 bytes)
    - X, Y, Z coordinates (12 bytes, floats)

- **Entity Destroy (`0x0A`)**: Notification that an entity has been destroyed (e.g., defeated monster, hit projectile).

  **Payload**:

    - Entity ID (4 bytes)

### 3. In-game Event Messages:

- **Game Event (`0x02`)**: Sends information about a specific in-game event (e.g., power-up collection, state change).

  **Payload**:

    - Event ID (2 bytes)
    - Event Data (variable size, depending on event type)

- **Power-Up Collected (`0x0B`)**: A player has picked up a power-up.

  **Payload**:

    - Player ID (4 bytes)
    - Power-Up Type (1 byte)

### 4. Server Management Messages:

- **Game Start (`0x0C`)**: Indicates that the game is starting and all players are ready.

  **Payload**:

    - Game ID (4 bytes)
    - Timestamp (4 bytes)

- **Game Over (`0x0D`)**: Indicates that the game has ended.

  **Payload**:

    - Winner Player ID (4 bytes)
    - Final Score (4 bytes)

- **Ping Request (`0x0E`)**: Ping request for checking latency or network connection.

  **Payload**: None

- **Ping Response (`0x0F`)**: Response to a ping request.

  **Payload**:

    - Timestamp (4 bytes)

### 5. Connection and Synchronization Messages:

- **Connection Request (`0x10`)**: Initial connection request from a client to the server.

  **Payload**:

    - Username (variable length)

- **Connection Accept (`0x11`)**: Server accepts the player's connection.

  **Payload**:

    - Player ID (4 bytes)

- **Connection Reject (`0x12`)**: Server rejects the player's connection.

  **Payload**:

    - Error Code (1 byte)
    - Message (variable length)

- **Synchronization (`0x13`)**: Synchronizes the game state between the server and clients (e.g., player positions, game status).

  **Payload**:

    - Game State Data (variable length)

### 6. Acknowledgment and Error Messages:

- **Acknowledgment (`0x04`)**: Confirms the reception of a critical packet (e.g., synchronization or important events).

  **Payload**:

    - Packet ID (2 bytes)

- **Error Message (`0x14`)**: Sends a notification in case of an error (e.g., invalid packet, wrong protocol version).

  **Payload**:

    - Error Code (1 byte)
    - Description (variable length)

### 7. Special Messages (Optional):

- **Chat Message (`0x15`)**: Sends a chat message between players.

  **Payload**:

    - Player ID (4 bytes)
    - Message (variable length)

- **Game Pause (`0x16`)**: A player or the server has paused the game.

  **Payload**:

    - Player ID (4 bytes)
    - Reason (1 byte)

### 8. Map Messages:

- **Map Data (`0x20`)**: Sends map data to the client.

  **Payload**:

    - Map ID (4 bytes)

- **Map Scroll (`0x21`)**: Updates the current scroll position of the map.

  **Payload**:

    - Scroll Position (4 bytes, X coordinate)

- **Map Tile Destroy (`0x22`)**: Informs that a tile has been destroyed.

  **Payload**:

    - X, Y coordinates (8 bytes)

---
## Summary Table

| Message Type            | Code (Hex) | Description                              | Payload                                                   |
|-------------------------|------------|------------------------------------------|-----------------------------------------------------------|
| Player Movement         | 0x01       | Updates player position                  | Player ID (4), X, Y, Z coordinates (4, 4, 4)               |
| Player Shoot            | 0x05       | Player shoots a missile                  | Player ID (4), Direction (1), Weapon type (1)              |
| Player Health Update    | 0x06       | Updates player's health                  | Player ID (4), Health (1)                                 |
| Player Respawn          | 0x07       | Player respawns                          | Player ID (4), X, Y, Z coordinates (4, 4, 4)               |
| Player Disconnect       | 0x08       | Player disconnection                     | Player ID (4)                                             |
| Entity Update           | 0x03       | Updates an entity                        | Entity ID (4), Entity Type (2), X, Y, Z coordinates (4, 4, 4) |
| Entity Spawn            | 0x09       | Creates a new entity                     | Entity ID (4), Entity Type (2), X, Y, Z coordinates (4, 4, 4) |
| Entity Destroy          | 0x0A       | Notifies entity destruction              | Entity ID (4)                                             |
| Game Event| 0x02       | Sends a specific game event              | Event ID (2), Event Data (variable)                       |
| Power-Up Collected      | 0x0B       | Player collects a power-up               | Player ID (4), Power-Up Type (1)                          |
| Game Start              | 0x0C       | Game starts                              | Game ID (4), Timestamp (4)                                |
| Game Over               | 0x0D       | Game ends                                | Winner Player ID (4), Final Score (4)                     |
| Ping Request            | 0x0E       | Checks network connection latency        | None                                                      |
| Ping Response           | 0x0F       | Response to a ping request               | Timestamp (4)                                             |
| Connection Request      | 0x10       | Client's initial connection request      | Username (variable)                                       |
| Connection Accept       | 0x11       | Server accepts player's connection       | Player ID (4)                                             |
| Connection Reject       | 0x12       | Server rejects the connection            | Error Code (1), Message (variable)                        |
| Synchronization         | 0x13       | Synchronizes game state                  | Game State Data (variable)                                |
| Acknowledgment          | 0x04       | Confirms packet reception                | Packet ID (2)                                             |
| Error Message           | 0x14       | Sends error notification                 | Error Code (1), Description (variable)                    |
| Chat Message            | 0x15       | Player chat message                      | Player ID (4), Message (variable)                         |
| Game Pause              | 0x16       | Game is paused                           | Player ID (4), Reason (1)                                 |
| Map Data                | 0x20       | Receives the map                         | Map ID (4)                                                |
| Map Scroll              | 0x21       | Advances in the map                      | X Position (4)                                            |
| Map Tile Destroy        | 0x22       | Informs tile destruction                 | X, Y coordinates (8 bytes)                                |

---

