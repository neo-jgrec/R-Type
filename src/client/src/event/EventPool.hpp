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
    #include <queue>

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
        /**
         * @brief Adds a new event to the pool.
         *
         * This method allows threads to push new events into the pool. It is thread-safe and
         * uses a mutex to ensure that concurrent access is properly synchronized.
         *
         * @param event The event to be added to the pool.
         */
        void pushEvent(const Event& event);

        /**
         * @brief Retrieves the next event from the pool, if available.
         *
         * This method pops an event from the pool. If the pool is empty, it blocks until
         * an event is available.
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


        void handler(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint);
    private:
        std::queue<Event> eventQueue; ///< Queue to store events.
        mutable std::mutex eventMutex; ///< Mutex to ensure thread-safety.
        std::condition_variable condition; ///< Condition variable to notify when events are added.
};

#endif // EVENTPOOL_HPP
