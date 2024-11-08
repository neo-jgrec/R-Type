#ifndef EVENTPOOL_HPP
    #define EVENTPOOL_HPP

#include <asio/ip/udp.hpp>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <deque>
#include <vector>

#include "../../../core/network/NetworkService.hpp"
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
        EventPool() = default;
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
        static void handler(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint);

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

    /**
     * @brief Retrieves the next event in the event queue and removes it.
     *
     * This method retrieves the next event from the event queue(and delete it's). It throws an exception if the event queue is empty.
     *
     * @return The next event in the queue.
     * @throws std::runtime_error If the event queue is empty.
     */
    Event getNextEvent();

    /**
     * @brief Retrieves all events from the event queue.
     *
     * This method returns all events currently in the event queue and clears the queue after retrieval.
     *
     * @return A vector containing all the events in the queue.
     */
    std::vector<Event> getAllEvents();

    /**
     * @brief Deletes an event from the event queue.
     *
     * This method removes the specified event from the event queue.
     *
     * @param event The event to be deleted from the queue.
     */
    void deleteEvent(Event& event);
    private:
        std::deque<Event> eventQueue; ///< Queue to store events.
        mutable std::mutex eventMutex; ///< Mutex to ensure thread-safety.
        std::condition_variable condition; ///< Condition variable to notify when events are added.
        std::optional<std::function<void(Event)>> _handler; ///< Optional handler function for processing events.
};
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
void setHandlers();


#endif // EVENTPOOL_HPP
