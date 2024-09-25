/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "Window/Window.hpp"
#include "Exceptions.hpp"
#include <iostream>

int main(int argc, char **argv) {
    try {
        if (argc != 2)
            throw Editor::Exception("Usage: ./editor <map_path>");
        Editor::Window window("R-Type Map Editor", argv[1]);

        window.run();
    } catch (const Editor::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
