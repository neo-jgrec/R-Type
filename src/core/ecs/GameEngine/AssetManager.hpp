#ifndef ASSETMANAGER_HPP_
#define ASSETMANAGER_HPP_

#ifndef GE_USE_SDL
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include <filesystem>

namespace core {

/**
 * @class AssetManager
 * @brief Manages the loading and retrieval of game assets such as textures, fonts, sounds, and music.
 *
 * This class provides methods to load assets from files and store them in memory for quick access
 * during game runtime. It uses SFML or SDL's multimedia classes to handle different asset types.
 */
class AssetManager {
public:
    #ifdef GE_USE_SDL
    AssetManager(SDL_Renderer* renderer) : renderer(renderer) {}
    #else
    AssetManager() = default;
    #endif

    #ifdef GE_USE_SDL
    ~AssetManager() {
        // Free SDL resources
        for (auto& [_, texture] : textures) SDL_DestroyTexture(texture);
        for (auto& [_, sound] : sounds) Mix_FreeChunk(sound);
        for (auto& [_, music] : music) Mix_FreeMusic(music);
        for (auto& [_, font] : fonts) TTF_CloseFont(font);
    }
    #endif

    /**
     * @brief Loads a texture from a file and stores it with the given name.
     * @param name The identifier for the texture.
     * @param filename The path to the texture file.
     * @throws std::runtime_error if the texture fails to load.
     */
    void loadTexture(const std::string& name, const std::string& filename) {
        std::string absolutePath = std::filesystem::absolute(filename).string();

        #ifndef GE_USE_SDL
        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(absolutePath)) {
            throw std::runtime_error("Failed to load texture: " + absolutePath);
        }
        textures[name] = texture;
        #else
        SDL_Surface* surface = IMG_Load(absolutePath.c_str());
        if (!surface) {
            throw std::runtime_error("Failed to load texture: " + absolutePath + " SDL_image Error: " + IMG_GetError());
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            throw std::runtime_error("Failed to create texture: " + absolutePath + " SDL Error: " + SDL_GetError());
        }
        SDL_FreeSurface(surface);
        textures[name] = texture;
        #endif
    }

    /**
     * @brief Retrieves a previously loaded texture.
     * @param name The identifier of the texture to retrieve.
     * @return A shared pointer to the sf::Texture or SDL_Texture pointer.
     * @throws std::runtime_error if the texture is not found.
     */
    #ifndef GE_USE_SDL
    std::shared_ptr<sf::Texture> getTexture(const std::string& name) {
    #else
    SDL_Texture* getTexture(const std::string& name) {
    #endif
        auto found = textures.find(name);
        if (found == textures.end()) {
            throw std::runtime_error("Texture not found: " + name);
        }
        return found->second;
    }

    /**
     * @brief Loads a font from a file and stores it with the given name.
     * @param name The identifier for the font.
     * @param filename The path to the font file.
     * @throws std::runtime_error if the font fails to load.
     */
    void loadFont(const std::string& name, const std::string& filename) {
        std::string absolutePath = std::filesystem::absolute(filename).string();

        #ifndef GE_USE_SDL
        auto font = std::make_shared<sf::Font>();
        if (!font->loadFromFile(absolutePath)) {
            throw std::runtime_error("Failed to load font: " + absolutePath);
        }
        fonts[name] = font;
        #else
        TTF_Font* font = TTF_OpenFont(absolutePath.c_str(), 24);
        if (!font) {
            throw std::runtime_error("Failed to load font: " + absolutePath + " SDL_ttf Error: " + TTF_GetError());
        }
        fonts[name] = font;
        #endif
    }

    /**
     * @brief Retrieves a previously loaded font.
     * @param name The identifier of the font to retrieve.
     * @return A reference to the sf::Font or TTF_Font pointer.
     * @throws std::runtime_error if the font is not found.
     */
    #ifndef GE_USE_SDL
    sf::Font& getFont(const std::string& name) {
    #else
    TTF_Font* getFont(const std::string& name) {
    #endif
        auto found = fonts.find(name);
        if (found == fonts.end()) {
            throw std::runtime_error("Font not found: " + name);
        }
        // return *found->second;
        #ifndef GE_USE_SDL
        return *found->second;
        #else
        return found->second;
        #endif
    }

    /**
     * @brief Loads a sound from a file and stores it with the given name.
     * @param name The identifier for the sound.
     * @param filename The path to the sound file.
     * @throws std::runtime_error if the sound fails to load.
     */
    void loadSound(const std::string& name, const std::string& filename) {
        std::string absolutePath = std::filesystem::absolute(filename).string();

        #ifndef GE_USE_SDL
        auto sound = std::make_shared<sf::SoundBuffer>();
        if (!sound->loadFromFile(absolutePath)) {
            throw std::runtime_error("Failed to load sound: " + absolutePath);
        }
        sounds[name] = sound;
        #else
        Mix_Chunk* sound = Mix_LoadWAV(absolutePath.c_str());
        if (!sound) {
            throw std::runtime_error("Failed to load sound: " + absolutePath + " SDL_mixer Error: " + Mix_GetError());
        }
        sounds[name] = sound;
        #endif
    }

    /**
     * @brief Retrieves a previously loaded sound.
     * @param name The identifier of the sound to retrieve.
     * @return A shared pointer to the sf::SoundBuffer or Mix_Chunk pointer.
     * @throws std::runtime_error if the sound is not found.
     */
    #ifndef GE_USE_SDL
    std::shared_ptr<sf::SoundBuffer> getSound(const std::string& name) {
    #else
    Mix_Chunk* getSound(const std::string& name) {
    #endif
        auto found = sounds.find(name);
        if (found == sounds.end()) {
            throw std::runtime_error("Sound not found: " + name);
        }
        return found->second;
    }

    /**
     * @brief Loads a music file and stores it with the given name.
     * @param name The identifier for the music.
     * @param filename The path to the music file.
     * @throws std::runtime_error if the music fails to load.
     */
    void loadMusic(const std::string& name, const std::string& filename) {
        std::string absolutePath = std::filesystem::absolute(filename).string();

        #ifndef GE_USE_SDL
        auto music = std::make_shared<sf::Music>();
        if (!music->openFromFile(absolutePath)) {
            throw std::runtime_error("Failed to load music: " + absolutePath);
        }
        this->music[name] = music;
        #else
        Mix_Music* music = Mix_LoadMUS(absolutePath.c_str());
        if (!music) {
            throw std::runtime_error("Failed to load music: " + absolutePath + " SDL_mixer Error: " + Mix_GetError());
        }
        this->music[name] = music;
        #endif
    }

    /**
     * @brief Retrieves a previously loaded music file.
     * @param name The identifier of the music to retrieve.
     * @return A reference to the sf::Music or Mix_Music pointer.
     * @throws std::runtime_error if the music is not found.
     */
    #ifndef GE_USE_SDL
    sf::Music& getMusic(const std::string& name) {
    #else
    Mix_Music* getMusic(const std::string& name) {
    #endif
        auto found = music.find(name);
        if (found == music.end()) {
            throw std::runtime_error("Music not found: " + name);
        }
        #ifndef GE_USE_SDL
        return *found->second;
        #else
        return found->second;
        #endif
    }

private:
    #ifndef GE_USE_SDL
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts;
    std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> sounds;
    std::unordered_map<std::string, std::shared_ptr<sf::Music>> music;
    #else
    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, TTF_Font*> fonts;
    std::unordered_map<std::string, Mix_Chunk*> sounds;
    std::unordered_map<std::string, Mix_Music*> music;
    SDL_Renderer* renderer;
    #endif
};

} // namespace core

#endif // ASSETMANAGER_HPP_
