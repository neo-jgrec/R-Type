/*
** EPITECH PROJECT, 2024
** editor
** File description:
** MainMenuBar
*/

#ifndef MAINMENUBAR_HPP_
#define MAINMENUBAR_HPP_

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <functional>

namespace Editor {
    class MainMenuBar {
        public:
            MainMenuBar();
            void render();

            std::function<void()> onNewMap;
            std::function<void()> onOpenMap;
            std::function<void()> onSaveMap;
            std::function<void()> onExit;
            std::function<void()> onUndo;
            std::function<void()> onRedo;
            std::function<void()> onResetView;
            std::function<void()> onLoadTileSet;
            std::function<void()> onAbout;

        private:
            void renderFileMenu() const;
            void renderEditMenu() const;
            void renderViewMenu() const;
            void renderHelpMenu() const;
    };
}

#endif /* !MAINMENUBAR_HPP_ */