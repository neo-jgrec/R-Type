#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <memory>

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

struct ParallaxComponent {
    sf::Vector2f speed;
};

struct TextureComponent {
    std::shared_ptr<sf::Texture> texture;
};

// Identifiers

struct Player {};
struct Enemy {};
struct Projectile {};