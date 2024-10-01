#include "Server.hpp"
#include "Components.hpp"
#include "Systems.hpp"
#include "EntityFactory.hpp"

void Server::init()
{
    _registry.register_component<Network>();
    _registry.register_component<Position>();
    _registry.register_component<Velocity>();
    _registry.register_component<Orientation>();
    _registry.register_component<RectangleCollider>();
    _registry.register_component<World>();
    _registry.register_component<Player>();
    _registry.register_component<Enemy>();
    _registry.register_component<Projectile>();

    _world = EntityFactory::createWorld(_registry);

    Systems::worldSystem(_registry);
}

void Server::update()
{
    std::cout << "Updating server" << std::endl;
    _registry.run_system<World>();
}

void Server::run()
{
    while (true) {
        update();
        sleep(1);
    }
}
