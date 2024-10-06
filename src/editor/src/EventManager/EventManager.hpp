/*
** EPITECH PROJECT, 2024
** editor
** File description:
** EventManager
*/

#ifndef EVENTMANAGER_HPP_
#define EVENTMANAGER_HPP_

#include <SFML/Window.hpp>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Editor {
    class EventManager {
        public:
            using EventCallback = std::function<void(const sf::Event&)>;

            void registerEvent(sf::Event::EventType type, const EventCallback& callback);
            void handleEvent(const sf::Event &event) const;

        private:
            std::unordered_map<sf::Event::EventType, std::vector<EventCallback>> _callbacks;
    };
}

#endif /* !EVENTMANAGER_HPP_ */
