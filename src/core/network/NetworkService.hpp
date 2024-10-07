/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkService.hpp
*/

#pragma once


#include <asio.hpp>
#include <memory>
#include <string>
#include <thread>
#include "event/EventPool.hpp"

class NetworkingService {
public:
    /**
     * @brief Singleton instance of the NetworkingService.
     * @param server_ip IP address of the server (default: 127.0.0.1).
     * @param port Port number for UDP communication (default: 12345).
     * @return Singleton instance of the NetworkingService.
     */
    static NetworkingService& getInstance(const std::string& server_ip = "127.0.0.1", int port = 12345) {
        static NetworkingService instance(server_ip, port);
        return instance;
    }

    /**
     * @brief Starts the networking service.
     *
     * This function runs the asynchronous receive loop in a separate thread.
     */
    void start() {
        receive();
        networkThread = std::jthread([this]() {
            io_context_.run();
        });
        std::cout << "Network service started" << std::endl;
    }

    /**
     * @brief Stops the networking service.
     */
    void stop() {
        io_context_.stop();
        networkThread.request_stop();
        networkThread.join();
    }

    /**
    * @brief Blocks the calling thread until the network service thread finishes execution.
    *
    * This method waits for the network service thread to complete, ensuring all network operations have finished.
    */
    void attempt()
    {
        networkThread.join();
    }

    /**
     * @brief Get the EventPool instance.
     * @return Reference to the EventPool.
     */
    EventPool& getEventPool() {
        return eventPool;
    }

    /**
     * @brief Sends a request to a specific recipient.
     * @param recipient The IP address of the recipient.
     * @param port The port of the recipient.
     * @param messageType The message type.
     * @param payload Optional payload (default empty).
     */
    void sendRequest(const std::string& recipient, int port, uint8_t messageType, const std::vector<uint8_t>& payload = {}) {
        GDTPHeader header{};
        header.version = 0x01;
        header.messageType = messageType;
        header.packetId = std::chrono::system_clock::now().time_since_epoch().count();
        header.payloadSize = static_cast<uint16_t>(payload.size());

        std::vector<uint8_t> buffer = header.toBuffer();
        buffer.insert(buffer.end(), payload.begin(), payload.end());

        asio::ip::udp::endpoint destination(asio::ip::make_address(recipient), port);
        socket_.send_to(asio::buffer(buffer), destination);
    }

    void sendRequest(asio::ip::udp::endpoint& endpoint, uint8_t messageType, const std::vector<uint8_t>& payload = {}) {
        GDTPHeader header{};
        header.version = 0x01;
        header.messageType = messageType;
        header.packetId = std::chrono::system_clock::now().time_since_epoch().count();
        header.payloadSize = static_cast<uint16_t>(payload.size());

        std::vector<uint8_t> buffer = header.toBuffer();
        buffer.insert(buffer.end(), payload.begin(), payload.end());

        socket_.send_to(asio::buffer(buffer), endpoint);
    }

private:
    NetworkingService([[maybe_unused]] const std::string& server_ip, int port)
        : socket_(io_context_, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), eventPool(EventPool::getInstance())
    {
    }

    /**
     * @brief Asynchronous receive function to listen for incoming UDP packets.
     *
     * This function continues receiving packets in an infinite loop.
     */
    void receive() {
        socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
            [this](std::error_code ec, std::size_t bytes_received) {
                if (!ec && bytes_received > 0) {
                    try {
                        std::vector<uint8_t> packetData(recv_buffer_.begin(), recv_buffer_.begin() + bytes_received);

                        GDTPHeader header = GDTPHeader::fromBuffer(packetData);
                        std::vector<uint8_t> payload(recv_buffer_.begin() + HEADER_SIZE, recv_buffer_.begin() + bytes_received);
                        eventPool.handler(header, payload, remote_endpoint_);
                    } catch (std::exception & e) {
                        std::cerr << "Error in receive: " << e.what() << std::endl;
                    }
                }
                receive(); // Continue receiving
            });
    }

    asio::io_context io_context_;                           ///< ASIO IO context for async operations.
    asio::ip::udp::socket socket_;                          ///< UDP socket for communication.
    asio::ip::udp::endpoint remote_endpoint_;               ///< Remote endpoint for incoming packets.
    std::array<uint8_t, 1400> recv_buffer_;                 ///< Buffer for received data.
    EventPool& eventPool;                                   ///< Event pool for storing and processing events.
    std::jthread networkThread;                             ///< Thread running the IO context.
};
