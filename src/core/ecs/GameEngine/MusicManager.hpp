#pragma once
#include <SFML/Audio.hpp>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <string>

class MusicManager {
public:
    MusicManager() = default;

    void loadMusic(const std::string& state, const std::string& filePath) {
        auto music = std::make_unique<sf::Music>();
        std::string absolutePath = std::filesystem::absolute(filePath).string();
        if (music->openFromFile(absolutePath)) {
            _musicMap[state] = std::move(music);
        }
    }

    void playMusic(const std::string& state) {
        if (_currentState != state) {
            if (_musicMap.find(state) != _musicMap.end()) {
                stopMusic();  // Stop current music
                _currentState = state;
                _musicMap[state]->play();
            }
        }
    }

    void stopMusic() {
        if (_musicMap.find(_currentState) != _musicMap.end()) {
            _musicMap[_currentState]->stop();
        }
        _currentState.clear();
    }

    void setVolume(float volume) {
        for (auto& [state, music] : _musicMap) {
            music->setVolume(volume);
        }
    }

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> _musicMap;
    std::string _currentState;
};
