#ifndef EVENTFACTORY_HPP
#define EVENTFACTORY_HPP

#include "Server.hpp"

class EventFactory {
public:
    static void gameStarted(Server &server);
    static void playerConnected(Server &server);
    static void playerDisconnected(Server &server);
    static void playerMove(Server &server);
    static void playerShoot(Server &server);
};

#endif //EVENTFACTORY_HPP
