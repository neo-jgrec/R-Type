#ifndef EVENTFACTORY_HPP
    #define EVENTFACTORY_HPP

#include "EventPool.hpp"
#include "../core/network/includes/RequestHeader.hpp"
#include <unordered_map>
#include <functional>

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
    using EventHandler = std::function<Event(const GDTPHeader&, const std::vector<uint8_t>&)>;
    static const std::unordered_map<uint8_t, EventHandler> handlers;

    static Event handlePlayerConnect(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerDisconnect(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleGameStart(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleGameOver(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleMapScroll(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleTileDestroy(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerProjectileCreate(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerMissileCreate(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerProjectileDestroy(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerMissileDestroy(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerMove(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerCollide(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerHit(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handlePlayerDie(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleEnemySpawn(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleEnemyMove(const GDTPHeader& header, const std::vector<uint8_t>& payload);
    static Event handleEnemyDie(const GDTPHeader& header, const std::vector<uint8_t>& payload);
};

#endif // EVENTFACTORY_HPP
