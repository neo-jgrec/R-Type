#pragma once
#include <SFML/Audio.hpp>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <string>

/**
 * @class MusicManager
 * @brief Manages music playback for different game states.
 */
class MusicManager {
public:
    /**
     * @brief Default constructor for MusicManager.
     */
    MusicManager() = default;

    /**
     * @brief Loads music for a specific game state.
     * @param state The game state identifier.
     * @param filePath The path to the music file.
     */
    void loadMusic(const std::string& state, const std::string& filePath) {
        auto music = std::make_unique<sf::Music>();
        std::string absolutePath = std::filesystem::absolute(filePath).string();
        if (music->openFromFile(absolutePath)) {
            _musicMap[state] = std::move(music);
        }
    }

    /**
     * @brief Plays music for a specific game state.
     * @param state The game state identifier.
     */
    void playMusic(const std::string& state) {
        if (_currentState != state) {
            if (_musicMap.find(state) != _musicMap.end()) {
                stopMusic();  // Stop current music
                _currentState = state;
                _musicMap[state]->play();
            }
        }
    }

    /**
     * @brief Stops the currently playing music.
     */
    void stopMusic() {
        if (_musicMap.find(_currentState) != _musicMap.end()) {
            _musicMap[_currentState]->stop();
        }
        _currentState.clear();
    }

    /**
     * @brief Sets the volume for all loaded music.
     * @param volume The volume level (0.0f to 100.0f).
     */
    void setVolume(float volume) {
        for (auto& [state, music] : _musicMap) {
            music->setVolume(volume);
        }
    }

    /**
     * @brief Gets the current volume of the playing music.
     * @return The current volume level (0.0f to 100.0f).
     */
    float getVolume() {
        return _musicMap[_currentState]->getVolume();
    }

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> _musicMap; ///< Map of game states to music
    std::string _currentState; ///< Current game state
};
