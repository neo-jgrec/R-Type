#ifndef GAMEENGINECOMPONENTS_HPP_
#define GAMEENGINECOMPONENTS_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <memory>

#include "../Entity/Entity.hpp"

namespace core::ge {

    enum class AnimationState {
        Moving,
        MovingUp,
        MovingDown,
        Dying
    };

    struct KeyBinding {
        sf::Keyboard::Key moveUpKey = sf::Keyboard::Up;
        sf::Keyboard::Key moveDownKey = sf::Keyboard::Down;
        sf::Keyboard::Key moveLeftKey = sf::Keyboard::Left;
        sf::Keyboard::Key moveRightKey = sf::Keyboard::Right;
        sf::Keyboard::Key fireKey = sf::Keyboard::Space;
    };

    struct InputStateComponent {
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        bool fire = false;
    };

    struct DrawableComponent {
        sf::RectangleShape shape;
    };

    struct ClockComponent {
        sf::Clock clock;
        sf::Time elapsedTime;
    };

    struct TransformComponent {
        sf::Vector2f position;
        sf::Vector2f size;
        sf::Vector2f scale;
        float rotation{};
    };

    struct ClickableComponent {
        bool clicked;
        std::function<void()> onClick;
    };

    struct AnimationComponent {
        // std::vector<sf::IntRect> frames;
        std::map<AnimationState, std::vector<sf::IntRect>> animations;
        AnimationState currentState = AnimationState::Moving;
        float frameTime;
        float elapsedTime;
        unsigned int currentFrame = 0;
        bool loop = true;
    };

    struct ColorComponent {
        sf::Color color;
    };

    struct SoundComponent {
        sf::Sound sound;
        std::shared_ptr<sf::SoundBuffer> buffer;
        bool playOnce = true;
        bool isPlaying = false;
    };

    struct MusicComponent {
        sf::Music music;
    };

    struct TextureComponent {
        std::shared_ptr<sf::Texture> texture;
    };

    struct CollisionComponent {
        uint32_t collisionMask = 0;
        std::vector<sf::FloatRect> collisionBoxes;
        std::vector<std::pair<uint32_t, std::function<void(const ecs::Entity&, const ecs::Entity&)>>> onCollision = {};
    };
}

#endif /* !GAMEENGINECOMPONENTS_HPP_ */
