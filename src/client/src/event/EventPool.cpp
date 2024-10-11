/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventPool
*/

#include "EventPool.hpp"
#include <iostream>

#include "../../../core/network/NetworkService.hpp"
#include "EventFactory.hpp"


class NetworkingService;
EventPool & EventPool::getInstance(){
    static EventPool instance;
    return instance;
}

void EventPool::pushEvent(const Event& event) {
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push_back(event);
    condition.notify_one();
    if (_handler.has_value()) {
        (*_handler)(event);
    }
}

std::optional<Event> EventPool::popEvent() {
    std::lock_guard<std::mutex> lock(eventMutex);
    if (eventQueue.empty()) {
        return {};
    }

    Event event = eventQueue.front();
    eventQueue.pop_front();
    return event;
}

bool EventPool::isEmpty() const {
    std::lock_guard<std::mutex> lock(eventMutex);
    return eventQueue.empty();
}

void EventPool::handler(const GDTPHeader &header, const std::vector<uint8_t> &payload,
                       [[maybe_unused]] const asio::ip::udp::endpoint &client_endpoint) {
    try {
        EventPool::getInstance().pushEvent(EventFactory::createEvent(header, payload));
    } catch (const std::exception &e) {
        std::cerr << "Error in EventPool::Handler: " << e.what() << std::endl;
    }
}

void EventPool::setNewHandler(const std::function<void(Event)>& newHandler){
    _handler = newHandler;
}

void EventPool::deleteEvent(Event& event) {
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.erase(std::remove(eventQueue.begin(), eventQueue.end(), event), eventQueue.end());
}

Event EventPool::getNextEvent() {
    std::lock_guard<std::mutex> lock(eventMutex);
    if (eventQueue.empty()) {
        throw std::runtime_error("Event queue is empty");
    }

    Event event = eventQueue.front();
    eventQueue.pop_front();
    return event;
}

std::vector<Event> EventPool::getAllEvents() {
    std::lock_guard<std::mutex> lock(eventMutex);
    std::vector<Event> allEvents;

    allEvents.insert(allEvents.end(), eventQueue.begin(), eventQueue.end());
    eventQueue.clear();

    return allEvents;
}
/**
 * @brief Configures the handlers for all possible event types (0 to 255).
 *
 * This function retrieves the singleton instance of the `NetworkingService`
 * and registers a default handler for each possible event type, ranging from 0 to 255.
 * The default handler is set to `EventPool::handler`, which will process
 * the incoming events.
 *
 * @details
 * The `setHandlers` function iterates over the range of possible event types (0 to 255)
 * and uses the `addEvent` method of the `NetworkingService` to associate each event type
 * with a generic handler. This allows the `NetworkingService` to automatically
 * handle different types of events as they are received.
 *
 * @note
 * - This function is typically called during the initialization phase to ensure
 *   that all event types are properly handled.
 * - The `EventPool::handler` function is used as a generic handler for all event types.
 *   This can be customized later if specific handling is needed for certain event types.
 * - It assumes that `NetworkingService::getInstance()` is already properly initialized.
 *
 * @code
 * // Example usage:
 * setHandlers();
 * // This sets up handlers for all message types (0-255) using `EventPool::handler`.
 * @endcode
 *
 * @see NetworkingService::addEvent() for adding individual event handlers.
 * @see EventPool::handler for the default event handling logic.
 */
void setHandlers()
{
    NetworkingService &networkingService = NetworkingService::getInstance();

    for (int i = 0; i < 256; i++) {
        networkingService.addEvent(i, EventPool::handler);
    }
}

