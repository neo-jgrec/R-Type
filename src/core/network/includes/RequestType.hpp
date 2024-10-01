/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RequestType
*/

#ifndef REQUESTTYPE_HPP
    #define REQUESTTYPE_HPP
    #include <cstdint> // Pour les types uint8_t, uint16_t, uint64_t
    #include "EventTypeMapping.hpp"
    #include "InvalidEventConversion.hpp"

// Enum pour les types de message GDTP
enum class GDTPMessageType : uint8_t {
    #define X(name, value) name = value,
    EVENT_TYPE_LIST
    #undef X
};

/**
 * @brief Converts a uint8_t to the corresponding EventType.
 *
 * Dynamically generated based on the EVENT_TYPE_LIST macro.
 *
 * @param value The uint8_t value representing the event type.
 * @return The corresponding EventType.
 * @throws std::invalid_argument If the value does not correspond to any valid EventType.
 */
inline GDTPMessageType uint8ToEventType(uint8_t value) {
    switch (value) {
        #define X(name, value) case value: return GDTPMessageType::name;
        EVENT_TYPE_LIST
        #undef X
        default:
            throw InvalidEventConversion(value);
    }
}

/**
 * @brief Converts an EventType to its corresponding uint8_t value.
 *
 * @param eventType The EventType to convert.
 * @return The corresponding uint8_t value.
 */
inline uint8_t eventTypeToUint8(GDTPMessageType eventType) {
    return static_cast<uint8_t>(eventType);
}

// Surcharge de l'opérateur de conversion explicite vers uint8_t
inline uint8_t operator+(GDTPMessageType messageType) {
    return static_cast<uint8_t>(messageType);
}

#endif //REQUESTTYPE_HPP
