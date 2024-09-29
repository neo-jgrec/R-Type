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
    eventQueue.push_back(event);
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
    std::lock_guard<std::mutex> lock(eventMutex);
    if (eventQueue.empty()) {
        return {};
    }

    Event event = eventQueue.front(); // Récupère le premier élément
    eventQueue.erase(eventQueue.begin()); // Supprime le premier élément
    return event;
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
                        const asio::ip::udp::endpoint &client_endpoint) {
    try {
        Event::EventType type = uint8ToEventType(header.messageType);

        switch (type) {
        case Event::EventType::PlayerMovement:
            handlePlayerMovement(header, payload);
            break;

        case Event::EventType::PlayerShoot:
            handlePlayerShoot(header, payload);
            break;

        case Event::EventType::ChatMessage:
            handleChatMessage(header, payload);
            break;

            // Ajoutez des cas supplémentaires pour les autres types d'événements
        default:
            throw UnknownEvent(header.messageType);
            break;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error in EventPool::Handler: " << e.what() << std::endl;
    }
}

void EventPool::setNewHandler(const std::function<void(Event)>& newHandler){

    _handler = newHandler;
}

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



void EventPool::handlePlayerMovement(const GDTPHeader &header, const std::vector<uint8_t> &payload) {
    if (payload.size() < sizeof(PlayerMovement)) {
        std::cerr << "Invalid payload size for PlayerMovement" << std::endl;
        return;
    }

    PlayerMovement movement;
    std::memcpy(&movement.playerId, &payload[0], 4);
    std::memcpy(&movement.x, &payload[4], 4);
    std::memcpy(&movement.y, &payload[8], 4);
    std::memcpy(&movement.z, &payload[12], 4);

    Event event(Event::EventType::PlayerMovement, movement);

    // Verrouiller l'accès à la pool d'événements
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push_back(event);
}

void EventPool::handlePlayerShoot(const GDTPHeader &header, const std::vector<uint8_t> &payload) {
    if (payload.size() < sizeof(PlayerShoot)) {
        std::cerr << "Invalid payload size for PlayerShoot" << std::endl;
        return;
    }

    PlayerShoot shoot;
    std::memcpy(&shoot.playerId, &payload[0], 4);
    shoot.direction = payload[4];
    shoot.weaponType = payload[5];

    Event event(Event::EventType::PlayerShoot, shoot);

    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push_back(event);
}

void EventPool::handleChatMessage(const GDTPHeader &header, const std::vector<uint8_t> &payload) {
    if (payload.size() < 6) {
        std::cerr << "Invalid payload size for ChatMessage" << std::endl;
        return;
    }

    uint32_t playerId;
    uint16_t messageLength;
    std::memcpy(&playerId, &payload[0], 4);
    std::memcpy(&messageLength, &payload[4], 2);

    std::string message(payload.begin() + 6, payload.begin() + 6 + messageLength);

    ChatMessage chatMessage{playerId, message};

    Event event(Event::EventType::ChatMessage, chatMessage);

    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push_back(event);
}

void EventPool::deleteEvent(Event& event) {
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.erase(std::remove(eventQueue.begin(), eventQueue.end(), event), eventQueue.end());
}