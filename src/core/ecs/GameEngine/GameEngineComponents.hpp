#ifndef GAMEENGINECOMPONENTS_HPP_
#define GAMEENGINECOMPONENTS_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <memory>

#include "../Entity/Entity.hpp"

namespace core::ge {

/**
 * @enum AnimationState
 * @brief Enumerates the possible states for entity animations.
 * 
 * Represents different animation states an entity can be in, such as moving, dying, etc.
 */
enum class AnimationState {
    Moving,
    MovingUp,
    MovingDown,
    Dying
};

/**
 * @struct KeyBinding
 * @brief Stores the key bindings for player controls.
 * 
 * Contains the key mappings for movement and actions like firing.
 */
struct KeyBinding {
    sf::Keyboard::Key moveUpKey = sf::Keyboard::Up;
    sf::Keyboard::Key moveDownKey = sf::Keyboard::Down;
    sf::Keyboard::Key moveLeftKey = sf::Keyboard::Left;
    sf::Keyboard::Key moveRightKey = sf::Keyboard::Right;
    sf::Keyboard::Key fireKey = sf::Keyboard::Space;
};

/**
 * @struct InputStateComponent
 * @brief Tracks the current input state for player movement and actions.
 * 
 * Flags that are set based on whether the player is pressing movement keys or the fire key.
 */
struct InputStateComponent {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool fire = false;
};

/**
 * @struct DrawableComponent
 * @brief Contains the drawable shape for an entity.
 * 
 * This component holds the graphical representation (e.g., a rectangle shape) of the entity that can be rendered on screen.
 */
struct DrawableComponent {
    sf::RectangleShape shape;
};

/**
 * @struct ClockComponent
 * @brief Manages time tracking for an entity.
 * 
 * Stores a clock and elapsed time, typically used for time-sensitive operations like animations or cooldowns.
 */
struct ClockComponent {
    sf::Clock clock;
    sf::Time elapsedTime;
};

/**
 * @struct TransformComponent
 * @brief Defines the position, size, scale, and rotation of an entity.
 * 
 * This component holds the transform data, used to control where and how the entity appears in the game world.
 */
struct TransformComponent {
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Vector2f scale;
    float rotation = 0.0f;
};

/**
 * @struct ClickableComponent
 * @brief Defines a clickable entity, including its clicked state and the action to perform when clicked.
 */
struct ClickableComponent {
    bool clicked = false;
    bool hovered = false;
    std::function<void()> onClick;
};

/**
 * @struct AnimationComponent
 * @brief Manages entity animations, including animation frames and states.
 * 
 * This component stores animations for different states (e.g., moving, dying) and tracks the current frame and elapsed time between frames.
 */
struct AnimationComponent {
    std::map<AnimationState, std::vector<sf::IntRect>> animations;
    AnimationState currentState = AnimationState::Moving;
    float frameTime;
    float elapsedTime;
    unsigned int currentFrame = 0;
    bool loop = true;
};

/**
 * @struct ColorComponent
 * @brief Represents the color of an entity.
 * 
 * This component allows changing the color of an entity, which could be useful for distinguishing players or states.
 */
struct ColorComponent {
    sf::Color color;
};

/**
 * @struct SoundComponent
 * @brief Stores and manages sound playback for an entity.
 * 
 * This component includes a sound object and its associated buffer, and tracks whether the sound should play once or loop.
 */
struct SoundComponent {
    sf::Sound sound;
    std::shared_ptr<sf::SoundBuffer> buffer;
    bool playOnce = true;
    bool isPlaying = false;
};

/**
 * @struct MusicComponent
 * @brief Handles background music playback for an entity.
 */
struct MusicComponent {
    sf::Music music;
};

/**
 * @struct TextureComponent
 * @brief Stores the texture applied to an entity.
 * 
 * This component holds a shared pointer to the texture used by the entity for rendering.
 */
struct TextureComponent {
    std::shared_ptr<sf::Texture> texture;
};

/**
 * @struct CollisionComponent
 * @brief Manages collision data and responses for an entity.
 * 
 * Contains collision masks and bounding boxes used for collision detection, as well as a list of functions to execute upon collision with specific entities.
 */
struct CollisionComponent {
    uint32_t collisionMask = 0;
    std::vector<sf::FloatRect> collisionBoxes;
    std::vector<std::pair<uint32_t, std::function<void(const ecs::Entity&, const ecs::Entity&)>>> onCollision = {};
};

/**
 * @struct TextComponent
 * @brief Manages text rendering for an entity.
 * 
 * This component stores an SFML text object and font, allowing the entity to display text in the game.
 */
struct TextComponent {
    sf::Text text;
    sf::Font font;
};

/**
 * @struct SceneComponent
 * @brief Associates an entity with a specific scene.
 * 
 * This component assigns an entity to a scene, identified by an integer name. It allows managing which entities belong to which scenes.
 */
struct SceneComponent {
    int sceneName;
};

/**
 * @struct TextInputComponent
 * @brief Component for text input fields.
 */
struct TextInputComponent {
    std::string text;
    std::string placeholder;
    bool isActive = false;
    size_t cursorPosition = 0;
    size_t maxLength = 100;
};

/**
 * @struct SliderComponent
 * @brief Component for slider entities.
 */
struct SliderComponent {
    float minValue;
    float maxValue;
    float currentValue;
    sf::RectangleShape bar;
    sf::CircleShape handle;
    std::function<void(float)> onChange;
};

} // namespace core::ge

#endif /* !GAMEENGINECOMPONENTS_HPP_ */
