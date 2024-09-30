/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventPool
*/

#pragma once

#ifndef EVENTPOOL_HPP
#define EVENTPOOL_HPP

    #include <asio/ip/udp.hpp>
    #include <condition_variable>
    #include <mutex>
    #include <optional>
    #include <deque>
    #include <map>
    #include <vector>

    #include "../core/network/includes/RequestHeader.hpp"
    #include "Event.hpp"

/**
 * @class EventPool
 * @brief A thread-safe pool that stores and manages game events or network events.
 *
 * The EventPool allows multiple threads to safely add events and retrieve them
 * for processing, ensuring that there are no race conditions.
 */
class EventPool {
    public:
        EventPool();
        static EventPool& getInstance();

        /**
         * @brief Adds a new event to the pool.
         *
         * @param event The event to be added to the pool.
         */
        void pushEvent(const Event& event);

        /**
         * @brief Retrieves the next event from the pool, if available.
         *
         * @return The next event in the pool. If no event is available, it blocks until an event is pushed.
         */
        std::optional<Event> popEvent();

        /**
         * @brief Checks if the event pool is empty.
         *
         * @return True if the pool is empty, false otherwise.
         */
        bool isEmpty() const;

        /**
         * @brief Event handler function for processing GDTP messages.
         *
         * @param header The header of the received message.
         * @param payload The payload of the message.
         * @param client_endpoint The client endpoint from which the message was received.
         */
        void handler(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint);

        /**
         * @brief Sets a new handler to be called when an event is pushed.
         *
         * @param newHandler The function to handle new events.
         */
        void setNewHandler(const std::function<void(Event)>& newHandler);

        /**
         * @brief Exception class for handling unknown events.
         */
        class UnknownEvent : public std::exception {
        public:
            UnknownEvent(uint8_t value) : _value(value) {}
            const char* what() const noexcept override {
                return std::string("Unknown event type: " + std::to_string(this->_value)).c_str();
            }
        private:
            uint8_t _value;
        };

        Event getNextEvent();
        std::vector<Event> getAllEvents();

        void deleteEvent(Event& event);

    private:
        std::deque<Event> eventQueue; ///< Queue to store events.
        mutable std::mutex eventMutex; ///< Mutex to ensure thread-safety.
        std::condition_variable condition; ///< Condition variable to notify when events are added.
        std::optional<std::function<void(Event)>> _handler;

        void handlePlayerMovement(const GDTPHeader &header, const std::vector<uint8_t> &payload);
        void handlePlayerShoot(const GDTPHeader &header, const std::vector<uint8_t> &payload);
        void handleChatMessage(const GDTPHeader &header, const std::vector<uint8_t> &payload);
        void handlePlayerHealthUpdate(const GDTPHeader &header, const std::vector<uint8_t> &payload);
        void handleEntitySpawn(const GDTPHeader &header, const std::vector<uint8_t> &payload);
        void handleEntityDestroy(const GDTPHeader &header, const std::vector<uint8_t> &payload);
        void handlePowerUpCollected(const GDTPHeader &header, const std::vector<uint8_t> &payload);
};

std::shared_ptr<std::map<uint8_t, std::function<void(
    const GDTPHeader &header, const std::vector<uint8_t> &payload,
    const asio::ip::udp::endpoint &client_endpoint)>>> handlersMapEventPool();

#endif // EVENTPOOL_HPP
