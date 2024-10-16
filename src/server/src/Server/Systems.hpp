#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "Server.hpp"

namespace Systems {
    void worldSystem(Server &server);
    void enemySystem(Server &server);
    void projectileSystem(Server &server);
};

#endif //SYSTEMS_HPP
