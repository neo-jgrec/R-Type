/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** HandlersReceive
*/

#pragma once

#ifndef HANDLERSRECEIVE_HPP
    #define HANDLERSRECEIVE_HPP
    #include <iostream>
    #include <vector>
    #include <cstring>
    #include <map>
    #include "../core/network/includes/RequestType.hpp"

namespace Receive {
    void handleConnectionRequest([[maybe_unused]] const GDTPHeader &header, [[maybe_unused]] const std::vector<uint8_t>& payload, [[maybe_unused]] const asio::ip::udp::endpoint& client_endpoint) {
        if (payload.empty()) {
            std::cerr << "Received Connection Request with an empty payload!" << std::endl;
            return;
        }
        std::string username(payload.begin(), payload.end());
        std::cout << "Received Connection Request from username: " << username << std::endl;
    }

    void handleConnectionAccept([[maybe_unused]] const GDTPHeader &header, [[maybe_unused]] const std::vector<uint8_t>& payload, [[maybe_unused]] const asio::ip::udp::endpoint& client_endpoint)
    {
        std::cout << "Received Connection Accept!" << std::endl;
    }

    std::shared_ptr<std::map<uint8_t, std::function<void(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint)>>> handlersMap()
    {
        auto handlers = std::make_shared<std::map<uint8_t, std::function<void(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint)>>>();

        handlers->emplace(+GDTPMessageType::ConnectionRequest, handleConnectionRequest);
        handlers->emplace(+GDTPMessageType::ConnectionAccept, handleConnectionAccept);
        return handlers;
    }
}

#endif //HANDLERSRECEIVE_HPP
