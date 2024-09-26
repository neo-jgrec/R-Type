/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventPool
*/

#include "EventPool.hpp"

#include <iostream>

#include "../core/network/includes/RequestType.hpp"

/**
 * @brief Adds a new event to the pool.
 *
 * This method allows threads to push new events into the pool. It is thread-safe and
 * uses a mutex to ensure that concurrent access is properly synchronized.
 *
 * @param event The event to be added to the pool.
 */
void EventPool::pushEvent(const Event& event) {
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push(event);
    condition.notify_one(); // Notifier que quelque chose a été ajouté
}

/**
 * @brief Retrieves the next event from the pool, if available.
 *
 * This method pops an event from the pool. If the pool is empty, it blocks until
 * an event is available.
 *
 * @return The next event in the pool. If no event is available, it blocks until an event is pushed.
 */
std::optional<Event> EventPool::popEvent() {
    std::unique_lock<std::mutex> lock(eventMutex);
    condition.wait(lock, [this] { return !eventQueue.empty(); }); // Attendre jusqu'à ce qu'il y ait un événement

    if (!eventQueue.empty()) {
        Event event = eventQueue.front();
        eventQueue.pop();
        return event;
    }

    return std::nullopt;
}

/**
 * @brief Checks if the event pool is empty.
 *
 * @return True if the pool is empty, false otherwise.
 */
bool EventPool::isEmpty() const {
    std::lock_guard<std::mutex> lock(eventMutex);
    return eventQueue.empty();
}

void EventPool::handler(const GDTPHeader &header, const std::vector<uint8_t> &payload,
                        const asio::ip::udp::endpoint &client_endpoint){
    try {
        Event::EventType type = uint8ToEventType(header.messageType);
    } catch (std::exception e) {
        std::cerr << "Error in EventPool::Handler: " << e.what() << std::endl;
    }
}


