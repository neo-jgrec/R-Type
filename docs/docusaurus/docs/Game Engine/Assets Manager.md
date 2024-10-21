# Asset Manager

The **Asset Manager** in the R-Type engine is a key component for managing game assets such as textures, fonts, sounds, and music. It allows assets to be loaded from files and stored in memory for quick retrieval, ensuring smooth and efficient game runtime.

## Why Use an Asset Manager?

### Advantages:
1. **Centralized Asset Handling**: All game assets are stored and managed in one place, which reduces redundancy and simplifies asset management.
2. **Memory Efficiency**: Assets are loaded only once and reused across the game, avoiding unnecessary memory usage and performance issues.
3. **Error Handling**: The Asset Manager provides clear error messages if assets fail to load, allowing developers to quickly troubleshoot issues.
4. **Code Cleanliness**: Having a dedicated manager for assets keeps the game code organized and modular.
5. **Scalability**: As the game grows, the Asset Manager can be expanded to support more types of assets and formats with minimal changes.

## Key Features

- **Texture Management**: Load and retrieve game textures.
- **Font Management**: Manage fonts for UI and in-game text.
- **Sound and Music**: Handle sound effects and music files for game audio.

## Asset Manager Class Overview

```cpp
namespace core {

/**
 * @class AssetManager
 * @brief Manages the loading and retrieval of game assets such as textures, fonts, sounds, and music.
 */
class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager() = default;

    void loadTexture(const std::string& name, const std::string& filename);
    std::shared_ptr<sf::Texture> getTexture(const std::string& name);

    void loadFont(const std::string& name, const std::string& filename);
    sf::Font& getFont(const std::string& name);

    void loadSound(const std::string& name, const std::string& filename);
    std::shared_ptr<sf::SoundBuffer> getSound(const std::string& name);

    void loadMusic(const std::string& name, const std::string& filename);
    sf::Music& getMusic(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts;
    std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> sounds;
    std::unordered_map<std::string, std::shared_ptr<sf::Music>> music;
};

} // namespace core
```

### Functions and Usage

| Function                  | Description                                                                                  |
| ------------------------- | -------------------------------------------------------------------------------------------- |
| `loadTexture(name, filename)` | Loads a texture from the specified file and stores it with an identifier `name`.           |
| `getTexture(name)`         | Retrieves the previously loaded texture by its identifier `name`.                            |
| `loadFont(name, filename)` | Loads a font from the specified file and stores it.                                          |
| `getFont(name)`            | Retrieves a loaded font by its identifier `name`.                                            |
| `loadSound(name, filename)`| Loads a sound file (e.g., .wav) and stores it for later retrieval.                           |
| `getSound(name)`           | Retrieves the loaded sound file by its identifier `name`.                                    |
| `loadMusic(name, filename)`| Loads a music file (streamed from file) and stores it with an identifier `name`.             |
| `getMusic(name)`           | Retrieves the loaded music by its identifier `name`.                                         |

### Example: Loading and Retrieving a Texture

```cpp
core::AssetManager assetManager;

// Load a texture
assetManager.loadTexture("spaceship", "assets/spaceship.png");

// Retrieve the texture later
std::shared_ptr<sf::Texture> spaceshipTexture = assetManager.getTexture("spaceship");

// Using the texture in a sprite
sf::Sprite spaceship;
spaceship.setTexture(*spaceshipTexture);
```

### Error Handling
Each loading function throws a `std::runtime_error` if the asset fails to load. This ensures that asset loading issues are caught and handled properly.

#### Example Error Handling

```cpp
try {
    assetManager.loadTexture("background", "assets/missing_background.png");
} catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

### Storage and Asset Management

The Asset Manager uses `std::unordered_map` to store assets efficiently, providing quick lookups by name. Each asset type (textures, fonts, sounds, music) is stored in a separate map.

- **Texture Storage**: Textures are stored in `std::unordered_map<std::string, std::shared_ptr<sf::Texture>>`.
- **Font Storage**: Fonts are stored as `std::unordered_map<std::string, std::shared_ptr<sf::Font>>`.
- **Sound Storage**: Sounds are managed in `std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>>`.
- **Music Storage**: Music files are stored in `std::unordered_map<std::string, std::shared_ptr<sf::Music>>`.

## Future Improvements

- **Streaming Textures**: Add support for streaming large textures directly from disk to reduce memory consumption.
- **Asset Pooling**: Implement an asset pooling system to reuse and recycle frequently used assets dynamically.
- **Async Loading**: Implement asynchronous loading for assets to prevent game lag during heavy asset loading times.

## Conclusion

The **Asset Manager** is essential in handling the different multimedia components of the R-Type game efficiently. It ensures smooth gameplay by reducing memory overhead and providing quick access to game assets, making development and asset management much more straightforward.