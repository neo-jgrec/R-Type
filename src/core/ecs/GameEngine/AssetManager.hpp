#ifndef ASSETMANAGER_HPP_
#define ASSETMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
 * during game runtime. It uses SFML's multimedia classes to handle different asset types.
 */
class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager() = default;

    /**
     * @brief Loads a texture from a file and stores it with the given name.
     * @param name The identifier for the texture.
     * @param filename The path to the texture file.
     * @throws std::runtime_error if the texture fails to load.
     */
    void loadTexture(const std::string& name, const std::string& filename) {
        auto texture = std::make_shared<sf::Texture>();
        std::string absolutePath = std::filesystem::absolute(filename).string();
        if (!texture->loadFromFile(absolutePath)) {
            throw std::runtime_error("Failed to load texture: " + absolutePath);
        }
        textures[name] = texture;
    }

    /**
     * @brief Retrieves a previously loaded texture.
     * @param name The identifier of the texture to retrieve.
     * @return A shared pointer to the sf::Texture.
     * @throws std::runtime_error if the texture is not found.
     */
    std::shared_ptr<sf::Texture> getTexture(const std::string& name) {
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
        auto font = std::make_shared<sf::Font>();
        std::string absolutePath = std::filesystem::absolute(filename).string();
        if (!font->loadFromFile(absolutePath)) {
            throw std::runtime_error("Failed to load font: " + absolutePath);
        }
        fonts[name] = font;
    }

    /**
     * @brief Retrieves a previously loaded font.
     * @param name The identifier of the font to retrieve.
     * @return A reference to the sf::Font.
     * @throws std::runtime_error if the font is not found.
     */
    sf::Font& getFont(const std::string& name) {
        auto found = fonts.find(name);
        if (found == fonts.end()) {
            throw std::runtime_error("Font not found: " + name);
        }
        return *found->second;
    }

    /**
     * @brief Loads a sound from a file and stores it with the given name.
     * @param name The identifier for the sound.
     * @param filename The path to the sound file.
     * @throws std::runtime_error if the sound fails to load.
     */
    void loadSound(const std::string& name, const std::string& filename) {
        auto sound = std::make_shared<sf::SoundBuffer>();
        std::string absolutePath = std::filesystem::absolute(filename).string();
        if (!sound->loadFromFile(absolutePath)) {
            throw std::runtime_error("Failed to load sound: " + absolutePath);
        }
        sounds[name] = sound;
    }

    /**
     * @brief Retrieves a previously loaded sound.
     * @param name The identifier of the sound to retrieve.
     * @return A shared pointer to the sf::SoundBuffer.
     * @throws std::runtime_error if the sound is not found.
     */
    std::shared_ptr<sf::SoundBuffer> getSound(const std::string& name) {
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
        auto music = std::make_shared<sf::Music>();
        std::string absolutePath = std::filesystem::absolute(filename).string();
        if (!music->openFromFile(absolutePath)) {
            throw std::runtime_error("Failed to load music: " + absolutePath);
        }
        this->music[name] = music;
    }

    /**
     * @brief Retrieves a previously loaded music file.
     * @param name The identifier of the music to retrieve.
     * @return A reference to the sf::Music.
     * @throws std::runtime_error if the music is not found.
     */
    sf::Music& getMusic(const std::string& name) {
        auto found = music.find(name);
        if (found == music.end()) {
            throw std::runtime_error("Music not found: " + name);
        }
        return *found->second;
    }


private:
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts;
    std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> sounds;
    std::unordered_map<std::string, std::shared_ptr<sf::Music>> music;
};

} // namespace core

#endif // ASSETMANAGER_HPP_
