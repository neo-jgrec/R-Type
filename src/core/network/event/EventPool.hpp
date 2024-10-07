/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventPool
*/


#pragma once

#ifndef EVENT_POOL_HPP
    #define EVENT_POOL_HPP
#include <algorithm>
#include <asio/ip/udp.hpp>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <queue>
#include "../NetworkService.hpp"
#include "../core/network/includes/RequestHeader.hpp"
#include "Event.hpp"
#include "EventFactory.hpp"

class EventPool {
public:
    EventPool() = default;

    static EventPool& getInstance() {
        static EventPool instance;
        return instance;
    }

    void pushEvent(const Event& event) {
        std::lock_guard<std::mutex> lock(eventMutex);
        eventQueue.push_back(event);
        condition.notify_one();
    }

    std::optional<Event> getNextEvent() {
        std::lock_guard<std::mutex> lock(eventMutex);
        if (eventQueue.empty()) {
            return std::nullopt;
        }

        Event event = eventQueue.front();
        eventQueue.pop_front();
        return event;
    }

    std::vector<Event> getAllEvents() {
        std::lock_guard<std::mutex> lock(eventMutex);
        std::vector<Event> events(eventQueue.begin(), eventQueue.end());
        eventQueue.clear();
        return events;
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(eventMutex);
        std::cout << "vector size empty: " << eventQueue.size() << std::endl;
        return eventQueue.empty();
    }
    EventPool& operator=(const EventPool& other) {
        if (this != &other) {
            std::lock_guard<std::mutex> lock(eventMutex);
            eventQueue = other.eventQueue;}
        return *this;
    }

    void handler(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& endpoint) {
        try {
            std::cout << "EventPool::handler receive Event: " << static_cast<int>(header.messageType) << std::endl;
            Event event = EventFactory::createEvent(header, payload, endpoint);
            pushEvent(event);
            std::cout << "vector size: " << eventQueue.size() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error in EventPool::handler: " << e.what() << std::endl;
        }
    }

private:
    std::deque<Event> eventQueue;
    mutable std::mutex eventMutex;
    std::condition_variable condition;
};

#endif //EVENT_POOL_HPP



