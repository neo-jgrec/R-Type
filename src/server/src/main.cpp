#include <iostream>
#include <fstream>
#include "../core/network/NetworkService.hpp"

int main() {
    // Serveur initialisé avec l'IP 0.0.0.0 (toutes les interfaces) et le port 12345
    NetworkingService serverService("0.0.0.0", 12345);

    // Lancement de l'écoute asynchrone dans un thread séparé
    serverService.run(); // Exécute le contexte Asio

    std::cout << "Server is running and listening on all interfaces (0.0.0.0:12345)" << std::endl;

    serverService.attempt();
    return 0;
}
