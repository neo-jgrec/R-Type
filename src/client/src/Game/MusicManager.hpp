#pragma once
#include <SFML/Audio.hpp>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <string>

class MusicManager {
public:
    MusicManager() = default;

    // Load music files and associate them with a state or level
    void loadMusic(const std::string& state, const std::string& filePath) {
        auto music = std::make_unique<sf::Music>();
        std::string absolutePath = std::filesystem::absolute(filePath).string();
        if (music->openFromFile(absolutePath)) {
            _musicMap[state] = std::move(music);
        }
    }

    // Play music for a given game state (e.g., a level)
    void playMusic(const std::string& state) {
        if (_currentState != state) {
            if (_musicMap.find(state) != _musicMap.end()) {
                stopMusic();  // Stop current music
                _currentState = state;
                _musicMap[state]->play();
            }
        }
    }

    // Stop the currently playing music
    void stopMusic() {
        if (_musicMap.find(_currentState) != _musicMap.end()) {
            _musicMap[_currentState]->stop();
        }
        _currentState.clear();
    }

    // Set volume for all music tracks
    void setVolume(float volume) {
        for (auto& [state, music] : _musicMap) {
            music->setVolume(volume);
        }
    }

private:
    // Use unique_ptr to manage the lifetime of sf::Music objects
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> _musicMap;
    std::string _currentState;  // Track the current state/level music
};
