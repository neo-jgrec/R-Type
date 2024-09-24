/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RequestHeader
*/

/**
 * @file RequestHeader.hpp
 * @brief Defines the GDTPHeader structure and utility functions for converting between host and network byte orders.
 */

#pragma once

#ifndef REQUESTHEADER_HPP
    #define REQUESTHEADER_HPP
    #include <cstdint>
    #include <vector>
    #include <cstring>

/**
 * @brief Converts a 64-bit unsigned integer from host byte order to network byte order.
 *
 * This function checks the system's endianness and swaps the bytes accordingly.
 *
 * @param value The 64-bit unsigned integer in host byte order.
 * @return The 64-bit unsigned integer in network byte order.
 *
 * @note This function is useful for sending 64-bit integers over the network.
 */
inline uint64_t htonll(uint64_t value)
{
    if (htonl(1) != 1) {
        return (static_cast<uint64_t>(htonl(value & 0xFFFFFFFF)) << 32) | htonl(value >> 32);
    }
    return value;
}

/**
 * @brief Converts a 64-bit unsigned integer from network byte order to host byte order.
 *
 * This function checks the system's endianness and swaps the bytes accordingly.
 *
 * @param value The 64-bit unsigned integer in network byte order.
 * @return The 64-bit unsigned integer in host byte order.
 *
 * @note This function is useful for receiving 64-bit integers from the network.
 */
inline uint64_t ntohll(uint64_t value) {
    if (ntohl(1) != 1) {
        return (static_cast<uint64_t>(ntohl(value & 0xFFFFFFFF)) << 32) | ntohl(value >> 32);
    }
    return value;
}

/**
 * @struct GDTPHeader
 * @brief Represents the header for a GDTP (Game Datagram Transfer Protocol) packet.
 *
 * The GDTP header consists of the following fields:
 * - `version` (1 byte): The protocol version.
 * - `messageType` (1 byte): The type of message being sent.
 * - `packetId` (8 bytes): A unique identifier for the packet.
 * - `payloadSize` (2 bytes): The size of the payload that follows the header.
 */
struct GDTPHeader {
    uint8_t version;       ///< The protocol version (1 byte).
    uint8_t messageType;   ///< The type of message (1 byte).
    uint64_t packetId;     ///< Unique identifier for the packet (8 bytes).
    uint16_t payloadSize;  ///< The size of the payload (2 bytes).

    /**
     * @brief Converts the GDTPHeader structure to a buffer for network transmission.
     *
     * This function serializes the header fields into a vector of bytes, converting
     * multi-byte fields (packetId and payloadSize) to network byte order.
     *
     * @return A vector of bytes representing the serialized header.
     *
     * @code
     * GDTPHeader header;
     * std::vector<uint8_t> buffer = header.toBuffer();
     * @endcode
     */
    std::vector<uint8_t> toBuffer() const {
        std::vector<uint8_t> buffer(12);  // 12 bytes for the header

        buffer[0] = version;
        buffer[1] = messageType;

        // Convert packetId to network byte order
        uint64_t packetIdNetworkOrder = htonll(packetId);
        std::memcpy(&buffer[2], &packetIdNetworkOrder, 8);

        // Convert payloadSize to network byte order
        uint16_t payloadSizeNetworkOrder = htons(payloadSize);
        std::memcpy(&buffer[10], &payloadSizeNetworkOrder, 2);

        return buffer;
    }

    /**
     * @brief Creates a GDTPHeader structure from a buffer received from the network.
     *
     * This function deserializes a buffer of bytes into a GDTPHeader structure, converting
     * multi-byte fields (packetId and payloadSize) from network byte order to host byte order.
     *
     * @param buffer A vector of bytes representing the received header.
     * @return A GDTPHeader structure populated with the values from the buffer.
     *
     * @note The input buffer must be at least 12 bytes long to contain a valid header.
     *
     * @code
     * std::vector<uint8_t> buffer = receiveBufferFromNetwork();
     * GDTPHeader header = GDTPHeader::fromBuffer(buffer);
     * @endcode
     */
    static GDTPHeader fromBuffer(const std::vector<uint8_t>& buffer) {
        GDTPHeader header;
        header.version = buffer[0];
        header.messageType = buffer[1];

        uint64_t packetIdNetworkOrder;
        std::memcpy(&packetIdNetworkOrder, &buffer[2], 8);
        header.packetId = ntohll(packetIdNetworkOrder);  // Convert to host byte order

        uint16_t payloadSizeNetworkOrder;
        std::memcpy(&payloadSizeNetworkOrder, &buffer[10], 2);
        header.payloadSize = ntohs(payloadSizeNetworkOrder);  // Convert to host byte order

        return header;
    }
};

#endif // REQUESTHEADER_HPP
