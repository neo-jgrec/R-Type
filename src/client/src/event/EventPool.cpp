/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventPool
*/

#include "EventPool.hpp"
#include <iostream>

#include "../core/network/includes/RequestType.hpp"
#include "EventFactory.hpp"

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
        EventFactory::createEvent(header, payload);
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
