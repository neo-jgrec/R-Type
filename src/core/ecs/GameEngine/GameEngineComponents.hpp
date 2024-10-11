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
     * @brief Represents different states of animation for game entities.
     */
    enum class AnimationState {
        Moving,
        MovingUp,
        MovingDown,
        Dying
    };

    /**
     * @struct KeyBinding
     * @brief Defines key bindings for player input.
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
     * @brief Represents the current input state for an entity.
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
     * @brief Component for entities that can be drawn on screen.
     */
    struct DrawableComponent {
        sf::RectangleShape shape;
    };

    /**
     * @struct ClockComponent
     * @brief Component for tracking time-related information for an entity.
     */
    struct ClockComponent {
        sf::Clock clock;
        sf::Time elapsedTime;
    };

    /**
     * @struct TransformComponent
     * @brief Component for storing position, size, scale, and rotation of an entity.
     */
    struct TransformComponent {
        sf::Vector2f position;
        sf::Vector2f size;
        sf::Vector2f scale;
        float rotation{};
    };

    /**
     * @struct ClickableComponent
     * @brief Component for entities that can be clicked.
     */
    struct ClickableComponent {
        bool clicked;
        std::function<void()> onClick;
    };

    /**
     * @struct AnimationComponent
     * @brief Component for entities with animations.
     */
    struct AnimationComponent {
        // std::vector<sf::IntRect> frames;
        std::map<AnimationState, std::vector<sf::IntRect>> animations;
        AnimationState currentState = AnimationState::Moving;
        float frameTime;
        float elapsedTime;
        unsigned int currentFrame = 0;
        bool loop = true;
    };

    /**
     * @struct ColorComponent
     * @brief Component for storing color information of an entity.
     */
    struct ColorComponent {
        sf::Color color;
    };

    /**
     * @struct SoundComponent
     * @brief Component for entities that can play sounds.
     */
    struct SoundComponent {
        sf::Sound sound;
        std::shared_ptr<sf::SoundBuffer> buffer;
        bool playOnce = true;
        bool isPlaying = false;
    };

    /**
     * @struct MusicComponent
     * @brief Component for entities that can play music.
     */
    struct MusicComponent {
        sf::Music music;
    };

    /**
     * @struct TextureComponent
     * @brief Component for entities with textures.
     */
    struct TextureComponent {
        std::shared_ptr<sf::Texture> texture;
    };

    /**
     * @struct CollisionComponent
     * @brief Component for entities that can collide with others.
     */
    struct CollisionComponent {
        uint32_t collisionMask = 0;
        std::vector<sf::FloatRect> collisionBoxes;
        std::vector<std::pair<uint32_t, std::function<void(const ecs::Entity&, const ecs::Entity&)>>> onCollision = {};
    };

    /**
     * @struct ButtonComponent
     * @brief Component for button entities.
     */
    struct ButtonComponent {
        sf::RectangleShape shape;
        std::function<void()> onClick;
        bool isHovered = false;
        bool isPressed = false;
    };

    /**
     * @struct TextComponent
     * @brief Component for entities with text.
     */
    struct TextComponent {
        sf::Text text;
        sf::Font font;
    };

    /**
     * @struct SceneComponent
     * @brief Component for managing scene information.
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
}

#endif /* !GAMEENGINECOMPONENTS_HPP_ */
