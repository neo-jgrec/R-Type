#pragma once
#include <SFML/Audio.hpp>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <string>

/**
 * @class MusicManager
 * @brief Manages the loading, playing, and stopping of background music tracks in the game.
 * 
 * The MusicManager class allows the game to handle multiple music tracks for different game states. 
 * It provides functionality to load music files, switch between tracks, and adjust the volume.
 */
class MusicManager {
public:
    /**
     * @brief Default constructor for the MusicManager.
     */
    MusicManager() = default;

    /**
     * @brief Loads a music file and associates it with a specific game state.
     * 
     * This method loads a music file from the specified file path and stores it in the music map. 
     * Each track is associated with a unique state identifier (e.g., "MainMenu", "Level1").
     *
     * @param state A string identifier for the game state or context in which the music will be played.
     * @param filePath The file path of the music file to load.
     * 
     * @note The file path is converted to an absolute path using `std::filesystem::absolute()`.
     * If the music file fails to load, it will not be added to the map.
     */
    void loadMusic(const std::string& state, const std::string& filePath) {
        auto music = std::make_unique<sf::Music>();
        std::string absolutePath = std::filesystem::absolute(filePath).string();
        if (music->openFromFile(absolutePath)) {
            _musicMap[state] = std::move(music);
        }
    }

    /**
     * @brief Plays the music associated with a given game state.
     * 
     * If a different track is currently playing, it stops the current track and starts playing the new one.
     * This method only plays music if the track for the given state is loaded.
     *
     * @param state The string identifier of the game state whose music should be played.
     * 
     * @note If the specified music is already playing, the function does nothing.
     * If the music for the given state is not loaded, the function will not change the music.
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
     * 
     * This method stops the music track associated with the current game state and clears the current state.
     */
    void stopMusic() {
        if (_musicMap.find(_currentState) != _musicMap.end()) {
            _musicMap[_currentState]->stop();
        }
        _currentState.clear();
    }

    /**
     * @brief Sets the volume for all loaded music tracks.
     * 
     * Adjusts the volume of all loaded music tracks to the specified level.
     * 
     * @param volume A float representing the desired volume level (0.0f to 100.0f).
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
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> _musicMap; ///< A map of game state identifiers to music tracks.
    std::string _currentState; ///< The identifier of the currently playing music state.
};
