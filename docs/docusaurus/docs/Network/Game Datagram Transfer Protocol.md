---
sidebar_position: 1
---

# Game Datagram Transfer Protocol (GDTP)
Binary Protocol for Client/Server Communications Using UDP

## Protocol Overview

The **Game Datagram Transfer Protocol (GDTP)** is a lightweight, low-latency binary protocol designed for real-time game communications over **UDP**. The protocol is optimized for the efficient transmission of game data such as player movement, entity updates, and events between clients and a server. Given **UDP**’s connectionless and unreliable nature, this protocol includes mechanisms to handle packet loss, packet ordering, and message validation without introducing significant overhead.

### Key Characteristics

- **Transport Layer**: **UDP** for all in-game communications.
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
```

### Integrity and Security:

- **Checksum**: Every packet includes a simple checksum in its header to ensure that the message has not been corrupted. The checksum is calculated by XOR-ing all bytes in the packet and verifying this at the receiver side.

- **Fixed-size Buffers**: Both the client and server use fixed-size buffers for receiving packets. If a packet exceeds the size defined by the `Payload Size` field, it is discarded to prevent buffer overflows.

### UDP-Specific Concerns:

1. **Handling Packet Loss**: The server can tolerate missing movement or event packets due to the state-based nature of the game. If the client sends regular updates, missing one or two packets doesn’t drastically affect the game state.

2. **Out-of-order Packets**: Each packet includes a `sequence number` to detect and reorder out-of-sequence packets.
