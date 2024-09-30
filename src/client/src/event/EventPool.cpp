/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventPool
*/

#include "EventPool.hpp"
#include <iostream>
#include "../core/network/includes/RequestType.hpp"

EventPool& EventPool::getInstance(){
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
                        const asio::ip::udp::endpoint &client_endpoint) {
    try {

        switch (Event::EventType type = uint8ToEventType(header.messageType)) {
        case Event::EventType::PlayerMovement:
            handlePlayerMovement(header, payload);
            break;
        case Event::EventType::PlayerShoot:
            handlePlayerShoot(header, payload);
            break;
        case Event::EventType::ChatMessage:
            handleChatMessage(header, payload);
            break;
        case Event::EventType::PlayerHealthUpdate:
            handlePlayerHealthUpdate(header, payload);
            break;
        case Event::EventType::EntitySpawn:
            handleEntitySpawn(header, payload);
            break;
        case Event::EventType::EntityDestroy:
            handleEntityDestroy(header, payload);
            break;
        case Event::EventType::PowerUpCollected:
            handlePowerUpCollected(header, payload);
            break;
        default:
            throw UnknownEvent(header.messageType);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error in EventPool::Handler: " << e.what() << std::endl;
    }
}

void EventPool::setNewHandler(const std::function<void(Event)>& newHandler){
    _handler = newHandler;
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

void EventPool::handlePlayerHealthUpdate(const GDTPHeader &header, const std::vector<uint8_t> &payload) {
    if (payload.size() < sizeof(PlayerHealthUpdate)) {
        std::cerr << "Invalid payload size for PlayerHealthUpdate" << std::endl;
        return;
    }

    PlayerHealthUpdate healthUpdate;
    std::memcpy(&healthUpdate.playerId, &payload[0], 4);
    std::memcpy(&healthUpdate.health, &payload[4], 4);

    Event event(Event::EventType::PlayerHealthUpdate, healthUpdate);
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push_back(event);
}

void EventPool::handleEntitySpawn(const GDTPHeader &header, const std::vector<uint8_t> &payload) {
    if (payload.size() < sizeof(EntitySpawn)) {
        std::cerr << "Invalid payload size for EntitySpawn" << std::endl;
        return;
    }

    EntitySpawn entitySpawn;
    std::memcpy(&entitySpawn.entityId, &payload[0], 4);
    std::memcpy(&entitySpawn.x, &payload[4], 4);
    std::memcpy(&entitySpawn.y, &payload[8], 4);

    Event event(Event::EventType::EntitySpawn, entitySpawn);
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push_back(event);
}

void EventPool::handleEntityDestroy(const GDTPHeader &header, const std::vector<uint8_t> &payload) {
    if (payload.size() < sizeof(EntityDestroy)) {
        std::cerr << "Invalid payload size for EntityDestroy" << std::endl;
        return;
    }

    EntityDestroy entityDestroy;
    std::memcpy(&entityDestroy.entityId, &payload[0], 4);

    Event event(Event::EventType::EntityDestroy, entityDestroy);
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push_back(event);
}

void EventPool::handlePowerUpCollected(const GDTPHeader &header, const std::vector<uint8_t> &payload) {
    if (payload.size() < sizeof(PowerUpCollected)) {
        std::cerr << "Invalid payload size for PowerUpCollected" << std::endl;
        return;
    }

    PowerUpCollected powerUpCollected;
    std::memcpy(&powerUpCollected.playerId, &payload[0], 4);
    std::memcpy(&powerUpCollected.powerUpId, &payload[4], 4);

    Event event(Event::EventType::PowerUpCollected, powerUpCollected);
    std::lock_guard<std::mutex> lock(eventMutex);
    eventQueue.push_back(event);
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

std::shared_ptr<std::map<uint8_t, std::function<void(
    const GDTPHeader &header, const std::vector<uint8_t> &payload,
    const asio::ip::udp::endpoint &client_endpoint)>>> handlersMapEventPool()
{
    auto handlers = std::make_shared<std::map<uint8_t,
    std::function<void(const GDTPHeader &header, const std::vector<uint8_t> &payload,
    const asio::ip::udp::endpoint &client_endpoint)>>>();

    #define X(name, value) \
    handlers->emplace(static_cast<uint8_t>(value), EventPool::getInstance().handler);

    EVENT_TYPE_LIST

    #undef X

    return handlers;
}
