#ifndef GAMEENGINECOMPONENTS_HPP_
#define GAMEENGINECOMPONENTS_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <memory>

#include "../Entity/Entity.hpp"

namespace core::ge {
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
        std::vector<sf::IntRect> frames;
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
        std::vector<sf::FloatRect> collisionBoxes;
        std::function<void(ecs::Entity, ecs::Entity)> onCollision = [](ecs::Entity, ecs::Entity) {};
    };
}

#endif /* !GAMEENGINECOMPONENTS_HPP_ */
