#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include "AssetManager.hpp"

#ifdef GE_USE_SDL
#include <SDL2/SDL.h>
#else
#include <SFML/Audio.hpp>
#endif

/**
 * @class MusicManager
 * @brief Manages the playing and stopping of background music tracks in the game.
 * 
 * The MusicManager class allows the game to handle multiple music tracks for different game states. 
 * It provides functionality to switch between tracks and adjust the volume.
 */
class MusicManager {
public:
    /**
     * @brief Default constructor for the MusicManager.
     */
    MusicManager() = default;

    /**
     * @brief Associates a loaded music track with a specific game state.
     * 
     * This method gets a music track from the AssetManager and associates it with a game state. 
     * Each track is associated with a unique state identifier (e.g., "MainMenu", "Level1").
     *
     * @param state A string identifier for the game state or context in which the music will be played.
     * @param musicName The identifier of the music in the AssetManager.
     * @param assetManager Reference to the AssetManager containing the music.
     */
    void addMusic(const std::string& state, const std::string& musicName, core::AssetManager& assetManager)
    {
        #ifdef GE_USE_SDL
            try {
                _musicMap[state] = assetManager.getMusic(musicName);
            } catch (const std::runtime_error& e) {
                std::cerr << "Failed to get music '" << musicName << "' from AssetManager: " << e.what() << std::endl;
            }
        #else
            try {
                _musicMap[state] = &assetManager.getMusic(musicName);
            } catch (const std::runtime_error& e) {
                std::cerr << "Failed to get music '" << musicName << "' from AssetManager: " << e.what() << std::endl;
            }
        #endif
    }

    /**
     * @brief Plays the music associated with a given game state.
     * 
     * If a different track is currently playing, it stops the current track and starts playing the new one.
     * This method only plays music if the track for the given state is loaded.
     *
     * @param state The string identifier of the game state whose music should be played.
     */
    void playMusic(const std::string& state) {
        #ifdef GE_USE_SDL
            if (_musicMap.find(state) == _musicMap.end()) {
                std::cerr << "Music for state '" << state << "' is not loaded!" << std::endl;
                return;
            }

            if (_currentState != state) {
                stopMusic();
                _currentState = state;
                Mix_VolumeMusic(_currentVolume);
                Mix_PlayMusic(_musicMap[state], -1);
            }
        #else
            if (_musicMap.find(state) == _musicMap.end()) {
                std::cerr << "Music for state '" << state << "' is not loaded!" << std::endl;
                return;
            }

            if (_currentState != state) {
                stopMusic();
                _currentState = state;
                _musicMap[state]->setVolume(_currentVolume);
                _musicMap[state]->play();
            }
        #endif
    }

    /**
     * @brief Stops the currently playing music.
     * 
     * This method stops the music track associated with the current game state and clears the current state.
     */
    void stopMusic()
    {
        #ifdef GE_USE_SDL
            if (!_currentState.empty()) {
                Mix_HaltMusic();
                _currentState.clear();
            }
        #else
            if (_musicMap.find(_currentState) != _musicMap.end()) {
                _musicMap[_currentState]->stop();
            }
            _currentState.clear();
        #endif
    }

    /**
     * @brief Sets the volume for all loaded music tracks.
     * 
     * Adjusts the volume of all loaded music tracks to the specified level.
     * 
     * @param volume A float representing the desired volume level (0.0f to 100.0f).
     */
    void setVolume(float volume)
    {
        #ifdef GE_USE_SDL
            _currentVolume = volume;
            Mix_VolumeMusic(volume);
        #else
            _currentVolume = volume;
            for (auto& [state, music] : _musicMap) {
                music->setVolume(volume);
            }
        #endif
    }

    /**
     * @brief Gets the current volume of the playing music.
     * @return The current volume level (0.0f to 100.0f).
     */
    float getVolume() const {
        return _currentVolume;
    }

private:
    #ifdef GE_USE_SDL
        std::unordered_map<std::string, Mix_Music*> _musicMap; ///< A map of game state identifiers to music tracks.
    #else
        std::unordered_map<std::string, sf::Music*> _musicMap; ///< A map of game state identifiers to music tracks.
    #endif
    std::string _currentState; ///< The identifier of the currently playing music state.
    float _currentVolume = 10;
};
