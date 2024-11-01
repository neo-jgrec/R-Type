#ifndef EVENTFACTORY_HPP
#define EVENTFACTORY_HPP

#include "Server.hpp"

namespace EventFactory {
    void gameStarted(Server &server);
    void playerConnected(Server &server);
    void playerDisconnected(Server &server);
    void playerMove(Server &server);
    void playerProjectileShoot(Server &server);
    void playerMissileShoot(Server &server);
};

#endif //EVENTFACTORY_HPP
