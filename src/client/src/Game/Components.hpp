#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>

struct KeyBinding {
    sf::Keyboard::Key moveUpKey = sf::Keyboard::Z;
    sf::Keyboard::Key moveDownKey = sf::Keyboard::S;
    sf::Keyboard::Key moveLeftKey = sf::Keyboard::Q;
    sf::Keyboard::Key moveRightKey = sf::Keyboard::D;
    sf::Keyboard::Key fireKey = sf::Keyboard::Space;
};

struct InputStateComponent {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool fire = false;
};

struct VelocityComponent {
    float dx, dy;
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

struct TextComponent {
    sf::Text text;
};

struct HealthComponent {
    int health;
};

struct DamageComponent {
    int damage;
};

struct ScoreComponent {
    int score;
};

struct ClickableComponent {
    bool clicked;
    std::function<void()> onClick;
};

struct CollisionComponent {
    std::vector<sf::FloatRect> collisionBoxes;
};

struct AnimationComponent {
    std::vector<sf::IntRect> frames;
    sf::Time frameTime;
    sf::Time elapsedTime;
    unsigned int currentFrame;
    bool loop;
};

struct ColorComponent {
    sf::Color color;
};

struct SoundComponent {
    sf::Sound soundEffect;
};

struct MusicComponent {
    sf::Music music;
};

struct ParallaxComponent {
    sf::Vector2f speed;
};
