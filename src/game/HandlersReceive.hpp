#ifndef HANDLERSRECEIVE_HPP
    #define HANDLERSRECEIVE_HPP

#include <iostream>
#include <vector>
#include <map>

namespace Receive {
    inline void handleConnectionRequest([[maybe_unused]] const GDTPHeader &header, [[maybe_unused]] const std::vector<uint8_t>& payload, [[maybe_unused]] const asio::ip::udp::endpoint& client_endpoint) {
        if (payload.empty()) {
            std::cerr << "Received Connection Request with an empty payload!" << std::endl;
            return;
        }
        std::string username(payload.begin(), payload.end());
        std::cout << "Received Connection Request from username: " << username << std::endl;
    }

    inline void handleConnectionAccept([[maybe_unused]] const GDTPHeader &header, [[maybe_unused]] const std::vector<uint8_t>& payload, [[maybe_unused]] const asio::ip::udp::endpoint& client_endpoint)
    {
        std::cout << "Received Connection Accept!" << std::endl;
    }

    inline std::shared_ptr<std::map<uint8_t, std::function<void(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint)>>> handlersMap()
    {
        auto handlers = std::make_shared<std::map<uint8_t, std::function<void(const GDTPHeader& header, const std::vector<uint8_t>& payload, const asio::ip::udp::endpoint& client_endpoint)>>>();

        handlers->emplace(0x10, handleConnectionRequest);
        handlers->emplace(0x11, handleConnectionAccept);
        return handlers;
    }
}

#endif //HANDLERSRECEIVE_HPP
