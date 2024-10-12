#include "EntityFactory.hpp"
#include "Game.hpp"

#include <SFML/Graphics.hpp>

#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <iostream>

#include "../../../game/Components.hpp"
#include "../../../game/CollisionMask.hpp"
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"
#include "Game.hpp"

core::ecs::Entity EntityFactory::createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position, int color, Game &game, sf::Vector2f gameScale)
{
    core::ecs::Entity player = registry.spawn_entity();

    registry.add_component(player, core::ge::TransformComponent{position, sf::Vector2f(33.0f, 17.0f), gameScale * 1.3f, 0.0f});
    registry.add_component(player, core::ge::CollisionComponent{PLAYER, {sf::FloatRect(0.0f, 0.0f, 33.0f, 17.0f)}, {
        { ENEMY, [&](const core::ecs::Entity self, [[maybe_unused]] const core::ecs::Entity other) {
                registry.kill_entity(self);
                game.releaseColor(color);
        }}}});
    registry.add_component(player, VelocityComponent{10.0f, 10.0f});
    registry.add_component(player, InputStateComponent{});
    registry.add_component(player, core::ge::KeyBinding{});
    registry.add_component(player, HealthComponent{10});
    registry.add_component(player, ScoreComponent{0});
    registry.add_component(player, Player{});
    registry.add_component(player, ShootCounterComponent{0});
    registry.add_component(player, PlayerColorComponent{color});

    std::string relativePath = "assets/player_sprite.png";
    std::string absolutePath = std::filesystem::absolute(relativePath).string();
    auto texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(absolutePath)) {
        std::cerr << "Failed to load texture: " << absolutePath << std::endl;
        return player;
    }

    sf::RectangleShape playerShape(sf::Vector2f(33.0f, 17.0f));
    playerShape.setTexture(texture.get());

    int row = color * 17;
    playerShape.setTextureRect(sf::IntRect(0, row, 33, 17));

    registry.add_component(player, core::ge::DrawableComponent{playerShape});
    registry.add_component(player, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});
    registry.add_component(player, core::ge::TextureComponent{texture});

    std::vector<sf::IntRect> moveFrames;
    moveFrames.reserve(5);
    for (int i = 0; i < 5; i++) {
        moveFrames.emplace_back(i * 33, row, 33, 17);
    }
    registry.add_component(player, core::ge::AnimationComponent{
        .animations = {
            {core::ge::AnimationState::Moving, moveFrames}
        },
        .frameTime = 0.1f,
        .elapsedTime = 0.0f,
        .currentFrame = 0,
        .loop = true
    });

    return player;
}

core::ecs::Entity EntityFactory::createPlayerProjectile(core::ecs::Registry& registry, core::ge::TransformComponent& playerTransform, sf::Vector2f gameScale)
{
    core::ecs::Entity projectile = registry.spawn_entity();

    sf::Vector2f projectileSize(18.0f, 5.0f);
    sf::Vector2f startPosition = playerTransform.position;

    float playerWidth = playerTransform.size.x * playerTransform.scale.x;
    float playerHeight = playerTransform.size.y * playerTransform.scale.y;
    startPosition.x += playerWidth;
    startPosition.y += (playerHeight / 2.0f) - (projectileSize.y / 2.0f);

    registry.add_component(projectile, core::ge::TransformComponent{startPosition, projectileSize, gameScale * 3.0f, 0.0f});
    registry.add_component(projectile, core::ge::CollisionComponent{PLAYER_PROJECTILE, {sf::FloatRect(0.0f, 0.0f, 18.0f, 5.0f)}});
    registry.add_component(projectile, VelocityComponent{20.0f, 20.0f});
    registry.add_component(projectile, DamageComponent{10});
    registry.add_component(projectile, Projectile{});

    auto buffer = std::make_shared<sf::SoundBuffer>();
    std::string soundPath = "assets/shooting_sound.ogg";
    if (!buffer->loadFromFile(soundPath)) {
        std::cerr << "Failed to load sound: " << soundPath << std::endl;
        return projectile;
    }

    sf::Sound sound;
    sound.setBuffer(*buffer);
    registry.add_component(projectile, core::ge::SoundComponent{sound, buffer, true, false});

    std::string relativePath = "assets/player_projectile.png";
    std::string absolutePath = std::filesystem::absolute(relativePath).string();
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(absolutePath)) {
        std::cerr << "Failed to load texture: " << absolutePath << std::endl;
        return projectile;
    }

    sf::RectangleShape projectileShape(sf::Vector2f(18.0f, 5.0f));
    projectileShape.setTexture(texture.get());
    projectileShape.setTextureRect(sf::IntRect(0, 0, 18, 5));
    registry.add_component(projectile, core::ge::DrawableComponent{projectileShape});
    registry.add_component(projectile, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});
    registry.add_component(projectile, core::ge::TextureComponent{texture});

    return projectile;
}

core::ecs::Entity EntityFactory::createPlayerMissile(core::ecs::Registry &registry, core::ge::TransformComponent &playerTransform, sf::Vector2f gameScale)
{
    core::ecs::Entity missile = registry.spawn_entity();

    sf::Vector2f missileSize(34.5f, 12.0f);
    sf::Vector2f scale = gameScale * 4.0f;
    sf::Vector2f startPosition = playerTransform.position;
    float playerWidth = playerTransform.size.x * playerTransform.scale.x;
    float playerHeight = playerTransform.size.y * playerTransform.scale.y;
    startPosition.x += playerWidth;
    startPosition.y += (playerHeight / 2.0f) - ((missileSize.y / 2.0f) * scale.y);


    registry.add_component(missile, core::ge::TransformComponent{startPosition, missileSize, scale, 0.0f});
    registry.add_component(missile, core::ge::CollisionComponent{PLAYER_MISSILE, {sf::FloatRect(0.0f, 0.0f, 34.5f, 12.0f)}});
    registry.add_component(missile, VelocityComponent{10.0f, 10.0f});
    registry.add_component(missile, DamageComponent{20});
    registry.add_component(missile, Projectile{});
    registry.add_component(missile, HealthComponent{50});

    auto buffer = std::make_shared<sf::SoundBuffer>();
    std::string soundPath = "assets/missile_sound.ogg";
    if (!buffer->loadFromFile(soundPath)) {
        std::cerr << "Failed to load sound: " << soundPath << std::endl;
        return missile;
    }

    sf::Sound sound;
    sound.setBuffer(*buffer);
    registry.add_component(missile, core::ge::SoundComponent{sound, buffer, true, false});

    std::string relativePath = "assets/player_missile.png";
    std::string absolutePath = std::filesystem::absolute(relativePath).string();
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(absolutePath)) {
        std::cerr << "Failed to load texture: " << absolutePath << std::endl;
        return missile;
    }

    sf::RectangleShape missileShape(sf::Vector2f(34.5f, 12.0f));
    missileShape.setTexture(texture.get());
    missileShape.setTextureRect(sf::IntRect(0, 0, 34, 12));
    registry.add_component(missile, core::ge::DrawableComponent{missileShape});
    registry.add_component(missile, core::ge::TextureComponent{texture});
    registry.add_component(missile, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});

    std::vector<sf::IntRect> moveFrames;
    moveFrames.reserve(2);
    for (int i = 0; i < 2; i++) {
        moveFrames.emplace_back(i * 34, 0, 34, 12);
    }
    registry.add_component(missile, core::ge::AnimationComponent{
        .animations = {
            {core::ge::AnimationState::Moving, moveFrames}
        },
        .frameTime = 0.1f,
        .elapsedTime = 0.0f,
        .currentFrame = 0,
        .loop = true
    });

    return missile;
}

core::ecs::Entity EntityFactory::createEnemy(core::ecs::Registry& registry, const sf::Vector2f& position, sf::Vector2f gameScale)
{
    core::ecs::Entity enemy = registry.spawn_entity();

    registry.add_component(enemy, core::ge::TransformComponent{position, sf::Vector2f(33.0f, 36.0f), gameScale, 0.0f});
    registry.add_component(enemy, core::ge::CollisionComponent{ENEMY, {sf::FloatRect(0.0f, 0.0f, 33.0f, 36.0f)}, {
        { PLAYER_PROJECTILE, [&](const core::ecs::Entity self, const core::ecs::Entity other) {
            const auto &damageDone = registry.get_components<DamageComponent>() [other];
            const auto &health = registry.get_components<HealthComponent>() [self];
            health->get()->health -= damageDone->get()->damage;
            if (health->get()->health <= 0) {
                registry.kill_entity(self);
            }
            registry.kill_entity(other);
        }},
        {PLAYER_MISSILE, [&](const core::ecs::Entity self, [[maybe_unused]] const core::ecs::Entity other) {
            const auto &damageDone = registry.get_components<DamageComponent>() [other];
            const auto &health = registry.get_components<HealthComponent>() [self];
            const auto &healthOther = registry.get_components<HealthComponent>() [other];
            health->get()->health -= damageDone->get()->damage;
            healthOther->get()->health -= damageDone->get()->damage;
            if (health->get()->health <= 0)
                registry.kill_entity(self);
            if (healthOther->get()->health <= 0)
                registry.kill_entity(other);
        }}
    }});
    registry.add_component(enemy, VelocityComponent{10.0f, 10.0f});
    registry.add_component(enemy, HealthComponent{10});
    registry.add_component(enemy, DamageComponent{10});
    registry.add_component(enemy, Enemy{});

    std::string relativePath = "assets/basic_enemy_sprite.png";
    std::string absolutePath = std::filesystem::absolute(relativePath).string();
    auto texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(absolutePath)) {
        std::cerr << "Failed to load texture: " << absolutePath << std::endl;
        return enemy;
    }

    sf::RectangleShape enemyShape(sf::Vector2f(33.0f, 36.0f));
    enemyShape.setTexture(texture.get());
    enemyShape.setTextureRect(sf::IntRect(0, 0, 33, 36));
    registry.add_component(enemy, core::ge::DrawableComponent{enemyShape});
    registry.add_component(enemy, core::ge::TextureComponent{texture});
    registry.add_component(enemy, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});
    std::vector<sf::IntRect> moveFrames;
    moveFrames.reserve(8);
    for (int i = 0; i < 5; i++) {
        moveFrames.emplace_back(i * 33, 0, 33, 36);
    }
    registry.add_component(enemy, core::ge::AnimationComponent{
        .animations = {
            {core::ge::AnimationState::Moving, moveFrames}
        },
        .frameTime = 0.1f,
        .elapsedTime = 0.0f,
        .currentFrame = 0,
        .loop = true
    });

    return enemy;
}

core::ecs::Entity EntityFactory::createButton(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void()>& onClick, int scene)
{
    core::ecs::Entity button = registry.spawn_entity();

    sf::RectangleShape shape(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);

    sf::Font font;
    if (!font.loadFromFile("assets/Fonts/Arial.ttf")) {
        std::cerr << "Failed to load font: " << "assets/Fonts/Arial.ttf" << std::endl;
        return button;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);

    registry.add_component(button, core::ge::TransformComponent{position, size, sf::Vector2f(1.0f, 1.0f), 0.0f});
    registry.add_component(button, core::ge::DrawableComponent{shape});
    registry.add_component(button, core::ge::TextComponent{text, font});
    registry.add_component(button, core::ge::ClickableComponent{false, false, onClick});
    registry.add_component(button, core::ge::SceneComponent{scene});
    return button;
}

core::ecs::Entity EntityFactory::createTextInput(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& placeholder, int scene)
{
    core::ecs::Entity textInput = registry.spawn_entity();

    sf::RectangleShape shape(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);

    sf::Font font;
    if (!font.loadFromFile("assets/Fonts/Arial.ttf")) {
        std::cerr << "Failed to load font: " << "assets/Fonts/Arial.ttf" << std::endl;
        return textInput;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(placeholder);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);

    registry.add_component(textInput, core::ge::DrawableComponent{shape});
    registry.add_component(textInput, core::ge::TextComponent{text, font});
    registry.add_component(textInput, core::ge::TextInputComponent{placeholder, placeholder, false, 0, 100});
    registry.add_component(textInput, core::ge::SceneComponent{scene});

    return textInput;
}

core::ecs::Entity EntityFactory::createSlider(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void(float)>& onChange, int scene, float currentValue)
{
    core::ecs::Entity slider = registry.spawn_entity();

    sf::RectangleShape bar(size);
    bar.setPosition(position);
    bar.setFillColor(sf::Color::White);
    bar.setOutlineThickness(2);
    bar.setOutlineColor(sf::Color::Black);

    sf::CircleShape handle(15);
    handle.setPosition(position.x + size.x, position.y + size.y / 2);
    handle.setFillColor(sf::Color::Red);

    sf::Font font;
    if (!font.loadFromFile("assets/Fonts/Arial.ttf")) {
        std::cerr << "Failed to load font: " << "assets/Fonts/Arial.ttf" << std::endl;
        return slider;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(position.x, position.y - 40.0f);

    registry.add_component(slider, core::ge::SliderComponent{0, 100, currentValue, bar, handle, onChange});
    registry.add_component(slider, core::ge::SceneComponent{scene});
    registry.add_component(slider, core::ge::TextComponent{text, font});

    return slider;
}

core::ecs::Entity EntityFactory::createImage(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& texturePath, int scene)
{
    core::ecs::Entity img = registry.spawn_entity();

    sf::RectangleShape shape(size);
    shape.setPosition(position);

    std::string absolutePath = std::filesystem::absolute(texturePath).string();
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(absolutePath)) {
        std::cerr << "Failed to load texture: " << absolutePath << std::endl;
        return img;
    }

    shape.setTexture(texture.get());

    registry.add_component(img, core::ge::DrawableComponent{shape});
    registry.add_component(img, core::ge::TextureComponent{texture});
    registry.add_component(img, core::ge::SceneComponent{scene});

    return img;
}
