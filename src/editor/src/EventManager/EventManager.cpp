/*
** EPITECH PROJECT, 2024
** editor
** File description:
** EventManager
*/

#include "EventManager.hpp"

using namespace Editor;

void EventManager::registerEvent(sf::Event::EventType type, const EventCallback& callback) {
    _callbacks[type].push_back(callback);
}

void EventManager::handleEvent(const sf::Event &event) const {
    auto it = _callbacks.find(event.type);
    if (it != _callbacks.end())
        for (const auto &callback : it->second)
            callback(event);
}
