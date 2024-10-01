---
title: Network Protocol
sidebar_position: 3
---

# **Game Datagram Transfer Protocol (GDTP)**
**Binary Protocol for Client/Server Communications Using UDP**

---

## **Protocol Overview**

The **Game Datagram Transfer Protocol (GDTP)** is a lightweight, binary protocol optimized for real-time game communication over **UDP**. It enables efficient transmission of game data such as player movements, entity updates, and game events between clients and a server.

Since **UDP** is connectionless and unreliable by nature, the GDTP protocol includes mechanisms to handle:
- **Packet Loss**
- **Packet Reordering**
- **Message Validation**  
  These mechanisms ensure robust communication without significant overhead.

---

## **Key Characteristics**

- **Transport Layer**: UDP is used for all in-game communications, prioritizing low-latency data exchange.

- **Secondary TCP Usage**: TCP is reserved for tasks such as authentication, connection setup, or transferring large data files (e.g., game assets). TCP is used where reliability is essential, but it is avoided for time-sensitive game data.

- **Packet-Oriented Communication**: Each message sent between the client and server is treated as a discrete **datagram** with its own header and payload. The payload is kept within typical **UDP limits (~1400 bytes)** to avoid fragmentation.

- **Error Handling & Security**:
    - **Malformed Packets**: Discarded to ensure the server/client remains stable.
    - **Buffer Overflow Protection**: Fixed-size buffers prevent memory-related security issues.

---

## **Packet Structure**

### **GDTP Packet**

Each GDTP packet consists of two main parts:
1. **Header (12 bytes)**
2. **Payload (variable size, depends on message type)**

### **Header Structure (12 bytes)**

| Field Name      | Size (bytes) | Description                                                       |
|-----------------|--------------|-------------------------------------------------------------------|
| **Version**     | 1            | The protocol version (currently `0x01`).                           |
| **Message Type**| 1            | Identifies the purpose of the message (e.g., `0x01` for player movement). |
| **Packet ID**   | 8            | A unique identifier for the packet (used for sequencing, deduplication). |
| **Payload Size**| 2            | Size of the payload, in bytes.                                     |
| **Sequence Number** | 2        | Used to track the order of packets. Helps reorder out-of-order packets. |
| **Total Packets**| 2           | Total number of packets in case the payload is split across multiple packets. |

### **Payload Structure (variable size)**

The **payload size** depends on the **message type** and the specific data being transmitted. Below are common message types and their corresponding payload formats.

---

## **Message Types**

The GDTP protocol defines several **message types**. Below are the most common types and their payload structures:

### **1. Player Movement (`0x01`)**
- **Description**: Sent by the client to update the server on the player's current position.

- **Payload Structure (12 bytes)**:
  | Field       | Size (bytes) |
  |-------------|--------------|
  | **Player ID**   | 4          |
  | **X Coordinate**| 4 (float)  |
  | **Y Coordinate**| 4 (float)  |
  | **Z Coordinate**| 4 (float)  |

### **2. Game Event (`0x02`)**
- **Description**: Represents various in-game events (e.g., pickups, interactions).

- **Payload Structure (Variable size)**:
  | Field        | Size (bytes) |
  |--------------|--------------|
  | **Event ID** | 2            |
  | **Event Data**| Variable     |

### **3. Entity Update (`0x03`)**
- **Description**: Sent by the server to update an entity's state in the game world.

- **Payload Structure (16 bytes)**:
  | Field         | Size (bytes) |
  |---------------|--------------|
  | **Entity ID**   | 4          |
  | **Entity Type** | 2          |
  | **X Coordinate**| 4 (float)  |
  | **Y Coordinate**| 4 (float)  |
  | **Z Coordinate**| 4 (float)  |

### **4. Acknowledgment (`0x04`)**
- **Description**: Used for critical updates where reliability is important (e.g., state synchronization).

- **Payload Structure (2 bytes)**:
  | Field         | Size (bytes) |
  |---------------|--------------|
  | **Packet ID**   | 2          |

---

## **Error Handling**

### **Malformed Packets**
- If a packet doesn't conform to the expected format (e.g., incorrect size or corrupted data), it is **discarded**. The server or client does not crash or consume additional memory when handling such packets.

### **Checksum Validation**
- A **checksum** is calculated using **XOR** over all bytes in the packet to ensure data integrity. This ensures that corrupted packets are detected and discarded early.

### **Buffer Overflow Protection**
- Fixed-size buffers (typically sized for maximum UDP datagram sizes of ~1400 bytes) ensure that packets exceeding the allowed size are discarded, preventing potential buffer overflow attacks.

### **Handling Packet Loss and Reordering**
- **UDP** does not guarantee packet delivery or ordering. Therefore, GDTP tracks packet sequence numbers and total packet counts to reorder messages if they arrive out of order.
- The game logic is designed to be **state-based**, so missing packets can be handled gracefully by applying updates from subsequent packets.

### **Duplicate Packets**
- The **Packet ID** field ensures that duplicate packets can be identified and ignored by the receiving end.

---

## **Protocol Example: Player Movement**

Here’s a sample **Player Movement Packet**:

| **Header Fields**       | Value           |
|-------------------------|-----------------|
| **Version**             | 1               |
| **Message Type**        | 1 (PlayerMovement) |
| **Packet ID**           | 1001            |
| **Payload Size**        | 12              |
| **Sequence Number**     | 1               |
| **Total Packets**       | 1               |

| **Payload Fields**      | Value           |
|-------------------------|-----------------|
| **Player ID**           | 12345           |
| **X Coordinate**        | 150.0           |
| **Y Coordinate**        | 200.0           |
| **Z Coordinate**        | 300.0           |

In this example, the player with **ID 12345** is sending their current **X, Y, and Z coordinates** to the server.

---

## **Protocol Documentation Summary**

The **Game Datagram Transfer Protocol (GDTP)** is designed to handle real-time game communications with minimal overhead, leveraging **UDP** for speed while adding mechanisms for reliability:
- **Packet Reordering**
- **Error Handling**  
  These features ensure reliable communication between the client and server in a game context, while being lightweight and efficient.

---