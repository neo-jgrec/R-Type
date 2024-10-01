/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventFactory.hpp
*/

#pragma once

#ifndef EVENTFACTORY_HPP
#define EVENTFACTORY_HPP

    #include "EventPool.hpp"
    #include "../core/network/includes/RequestHeader.hpp"

/**
 * @class EventFactory
 * @brief Factory class responsible for creating Event objects from network data.
 *
 * The EventFactory class provides static methods to convert GDTP headers and payload data
 * into fully constructed Event objects. This ensures separation of event creation logic from other components.
 */
class EventFactory {
public:
    /**
     * @brief Creates an Event object based on the GDTPHeader and payload data.
     *
     * This method converts the header and the payload into the appropriate Event type
     * depending on the messageType in the GDTPHeader. If the messageType does not match
     * any known event, it throws an UnknownEvent exception.
     *
     * @param header The GDTPHeader object containing metadata about the event.
     * @param payload The raw binary data representing the event's payload.
     * @return A fully constructed Event object corresponding to the GDTPMessageType.
     * @throws UnknownEvent if the messageType is invalid or unsupported.
     */
    static Event createEvent(const GDTPHeader& header, const std::vector<uint8_t>& payload);

private:
    // Handlers for each specific event type
    static Event handlePlayerMovement(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerShoot(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleChatMessage(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerHealthUpdate(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleEntitySpawn(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleEntityDestroy(const GDTPHeader& header, const std::vector<uint8_t>& payload);
};

#endif // EVENTFACTORY_HPP
