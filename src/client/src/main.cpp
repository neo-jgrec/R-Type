#include <iostream>
#include "../../core/ecs/Registry/Registry.hpp"
#include "../../core/network/NetworkService.hpp"

struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

void print_positions_and_velocities(const Position &position, const Velocity &velocity)
{
    std::cout << "Entity has Position (" << position.x << ", " << position.y << ") "
              << "and is moving at (" << velocity.dx << ", " << velocity.dy << ")\n";
}

int main()
{
    try {
        // Initialisation du client (adresse IP du serveur et port)
        NetworkingService clientService("127.0.0.1", 8080);
        clientService.run();

        // Envoi d'un message de mouvement du joueur
        std::vector<std::string> movementArgs = { "1", "100.0", "200.0", "300.0" };
        clientService.sendRequest("127.0.0.1", 12345,GDTPMessageType::PlayerMovement, movementArgs);

        // Attendre un peu avant d'envoyer le prochain message
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Envoi d'un message de tir du joueur
        std::vector<std::string> shootArgs = { "1", "2", "1" };
        clientService.sendRequest("127.0.0.1",  12345, GDTPMessageType::PlayerShoot, shootArgs);

        clientService.sendRequest("127.0.0.1",  12345, GDTPMessageType::ConnectionRequest);
        clientService.attempt();

        std::cout << "Client sent requests to 127.0.0.1:12345" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    core::ecs::Registry registry;

    registry.register_component<Position>();
    registry.register_component<Velocity>();

    auto entity1 = registry.spawn_entity();
    auto entity2 = registry.spawn_entity();

    registry.emplace_component<Position>(entity1, 1.0f, 2.0f);
    registry.emplace_component<Velocity>(entity1, 0.5f, 0.5f);

    registry.emplace_component<Position>(entity2, 3.0f, 4.0f);
    registry.emplace_component<Velocity>(entity2, 1.0f, 1.0f);

    registry.add_system<Position, Velocity>([](const Position &pos, const Velocity &vel)
                                            { print_positions_and_velocities(pos, vel); });

    registry.run_systems();
    return 0;
}
