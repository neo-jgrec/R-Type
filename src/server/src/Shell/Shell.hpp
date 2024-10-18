/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Shell
*/

#pragma once
#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <optional>

#include <vector>

#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#ifndef SHELL_HPP
    #define SHELL_HPP
#define TIMEOUT 1 // Timeout de 500ms pour poll()
class Shell {
public:
    Shell(bool &isRunning, bool &asGameStarted, std::array<std::optional<core::ecs::Entity>, 4> &players, core::ecs::Registry &registry) :
    _isRunning(isRunning), _asGameStarted(asGameStarted), _players(players), _registry(registry) {};
    ~Shell() = default;
    void update() const;
    void state([[maybe_unused]] std::string command) const;
     void listPlayers([[maybe_unused]] std::string command) const;
    void kick([[maybe_unused]] std::string command) const;
private:
    std::array<std::optional<core::ecs::Entity>, 4> &_players;
    bool &_asGameStarted;
    bool &_isRunning;
    std::map<std::string, std::function<void(std::string command)>> _commands = {
        {"state", [this](std::string command) { state(command); }},
        {"list players", [this](std::string command) { listPlayers(command); }},
        {"kick", [this](std::string command) { kick(command); }},
    };
    core::ecs::Registry &_registry;
    void check_command(const std::string &command) const;

};

#endif //SHELL_HPP

