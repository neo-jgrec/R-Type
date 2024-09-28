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
    if (_handler.has_value()) {
        (*_handler)(event);
    }
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
        //TODO implementé la création de l'event
    } catch (std::exception e) {
        std::cerr << "Error in EventPool::Handler: " << e.what() << std::endl;
    }
}

void EventPool::setNewHandler(const std::function<void(Event)>& newHandler){

    _handler = newHandler;
}

void EventPool::deleteEvent(Event &event) {}

std::shared_ptr<std::map<uint8_t, std::function<void(
    const GDTPHeader &header, const std::vector<uint8_t> &payload,
    const asio::ip::udp::endpoint &client_endpoint)>>> handlersMapEventPool()
{
    auto handlers = std::make_shared<std::map<uint8_t,
    std::function<void(const GDTPHeader &header, const std::vector<uint8_t> &payload,
    const asio::ip::udp::endpoint &client_endpoint)>>>();

    // Utiliser la macro pour enregistrer chaque type d'événement dans la map
    #define X(name, value) \
    handlers->emplace(static_cast<uint8_t>(value), EventPool::getInstance().handler);

    EVENT_TYPE_LIST

    #undef X

    return handlers;
}

EventPool &EventPool::getInstance(){
    static EventPool instance;
    return instance;
}




