#pragma once

#ifndef GE_USE_SDL
#include <SFML/Audio.hpp>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

#include <filesystem>
#include <iostream>
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
    MusicManager() {
        #ifdef GE_USE_SDL
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
        #endif
    }

    ~MusicManager() {
        #ifdef GE_USE_SDL
        for (auto& [_, music] : _musicMap) {
            Mix_FreeMusic(music);
        }
        Mix_CloseAudio();
        #endif
    }

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
        std::string absolutePath = std::filesystem::absolute(filePath).string();

        #ifndef GE_USE_SDL
        auto music = std::make_unique<sf::Music>();
        if (music->openFromFile(absolutePath)) {
            _musicMap[state] = std::move(music);
        } else {
            std::cerr << "Failed to load music file: " << absolutePath << std::endl;
        }
        #else
        Mix_Music* music = Mix_LoadMUS(absolutePath.c_str());
        if (music) {
            _musicMap[state] = music;
        } else {
            std::cerr << "Failed to load music file: " << absolutePath << " SDL_mixer Error: " << Mix_GetError() << std::endl;
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
     * 
     * @note If the specified music is already playing, the function does nothing.
     * If the music for the given state is not loaded, the function will not change the music.
     */
    void playMusic(const std::string& state) {
        if (_musicMap.find(state) == _musicMap.end()) {
            std::cerr << "Music for state '" << state << "' is not loaded!" << std::endl;
            return;
        }

        if (_currentState != state) {
            stopMusic();  
            _currentState = state;

            #ifndef GE_USE_SDL
            _musicMap[state]->play();
            #else
            Mix_PlayMusic(_musicMap[state], -1);
            #endif
        }
    }

    /**
     * @brief Stops the currently playing music.
     * 
     * This method stops the music track associated with the current game state and clears the current state.
     */
    void stopMusic() {
        #ifndef GE_USE_SDL
        if (_musicMap.find(_currentState) != _musicMap.end()) {
            _musicMap[_currentState]->stop();
        }
        #else
        Mix_HaltMusic();
        #endif
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
        #ifndef GE_USE_SDL
        for (auto& [state, music] : _musicMap) {
            music->setVolume(volume);
        }
        #else
        Mix_VolumeMusic(static_cast<int>(volume / 100.0f * MIX_MAX_VOLUME));
        #endif
    }

    /**
     * @brief Gets the current volume of the playing music.
     * @return The current volume level (0.0f to 100.0f).
     */
    float getVolume() {
        #ifndef GE_USE_SDL
        return _musicMap[_currentState]->getVolume();
        #else
        return static_cast<float>(Mix_VolumeMusic(-1)) / MIX_MAX_VOLUME * 100.0f;
        #endif
    }

private:
    #ifndef GE_USE_SDL
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> _musicMap; ///< A map of game state identifiers to SFML music tracks.
    #else
    std::unordered_map<std::string, Mix_Music*> _musicMap; ///< A map of game state identifiers to SDL music tracks.
    #endif
    std::string _currentState; ///< The identifier of the currently playing music state.
};
