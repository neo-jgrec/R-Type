/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Shell
*/

#include "Shell.hpp"

#include "../../../game/RequestType.hpp"
#include "src/Server/Components.hpp"


void Shell::update() const
{
    struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    char buffer[1024];

        int ret = poll(fds, 1, TIMEOUT);

        if (ret > 0) {
            if (fds[0].revents & POLLIN) {
                if (fgets(buffer, sizeof(buffer), stdin) != nullptr) {
                    check_command(buffer);
                    }
                }
            }
}

void Shell::check_command(const std::string &command) const
{
    std::string commandStr = command.substr(0, command.size() - 1);
    for (const auto &cmd : _commands) {
        std::cout << "=================================================" << std::endl;
        std::cout << "Command: " << commandStr.starts_with( cmd.first) << std::endl;
        if (commandStr.starts_with( cmd.first)) {
            std::cout << "=================================================" << std::endl;
            cmd.second(commandStr);
            return;
        }
    }
}


void Shell::state([[maybe_unused]] std::string command) const {
    std::cout << "Game state: " << (_asGameStarted ? "Started" : "Not started") << std::endl;
}

void Shell::listPlayers([[maybe_unused]] std::string command) const
{
    if (_players.empty() || _players[0].has_value() == false) {
        std::cout << "No player connected" << std::endl;
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (_players[i].has_value()) {
            const auto &playerComponent = _registry.get_component<Player>(_players[i].value());
            std::cout << "Player " << i << " connected" << std::endl;
            std::cout << "Player ID: " << static_cast<int>(playerComponent->id) << std::endl;
            std::cout << "Player IP: " << playerComponent->endpoint->address().to_string() << std::endl;
            std::cout << "Player Port: " << playerComponent->endpoint->port() << std::endl;
        }
    }
}

void Shell::kick([[maybe_unused]] std::string command) const
{
    NetworkingService &networking_service = NetworkingService::getInstance();
    std::string idStr = command.substr(5, command.size());
    int id = std::stoi(idStr);
    if (id < 0 || id > 3 || !_players[id].has_value()) {
        std::cout << "Player " << id << " not connected" << std::endl;
        return;
    }
    const auto &playerComponent = _registry.get_component<Player>(_players[id].value());
    if (playerComponent == nullptr) {
        std::cout << "Player " << id << " error" << std::endl;
        return;
    }
    std::cout << "Player ID: " << static_cast<int>(playerComponent->id) << std::endl;
    std::cout << "Player IP: " << playerComponent->endpoint->address().to_string() << std::endl;
    std::cout << "Player Port: " << playerComponent->endpoint->port() << std::endl;
    std::string ip = playerComponent->endpoint->address().to_string();
    int port = playerComponent->endpoint->port();
    networking_service.sendRequest(
        ip, port,
        RequestType::PlayerDisconnect);
    std::cout << "Player " << id << " kicked" << std::endl;
}


