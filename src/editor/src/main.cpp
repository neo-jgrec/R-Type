/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "Window/Window.hpp"
#include "Exceptions.hpp"
#include <iostream>

int main() {
    try {
        Editor::Window window("R-Type Map Editor");

        window.run();
    } catch (const Editor::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
