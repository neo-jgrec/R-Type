/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "Window/Window.hpp"
#include "Exceptions.hpp"
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

int main(int argc, char **argv) {
    try {
        // Determine the map path
        std::string mapPath = (argc > 1) ? argv[1] : "";

        std::cout << "mapPataah: " << mapPath << std::endl;

        Editor::Window window("R-Type Map Editor", mapPath);

        window.run();
    } catch (const Editor::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
