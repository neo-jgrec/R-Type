#include "EntityFactory.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include "../../../game/Components.hpp"
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"

core::ecs::Entity EntityFactory::createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position)
{
    core::ecs::Entity player = registry.spawn_entity();

    registry.add_component(player, core::ge::TransformComponent{position, sf::Vector2f(33.0f, 17.0f), sf::Vector2f(4.0f, 4.0f), 0.0f});
    registry.add_component(player, core::ge::CollisionComponent{.collisionBoxes = {sf::FloatRect(0.0f, 0.0f, 33.0f, 17.0f)}, .onCollision = nullptr});
    registry.add_component(player, VelocityComponent{10.0f, 10.0f});
    registry.add_component(player, InputStateComponent{});
    registry.add_component(player, core::ge::KeyBinding{});
    registry.add_component(player, HealthComponent{100});
    registry.add_component(player, ScoreComponent{0});
    registry.add_component(player, Player{});

    std::string relativePath = "assets/player_sprite.png";
    std::string absolutePath = std::filesystem::absolute(relativePath).string();
    auto texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(absolutePath)) {
        std::cerr << "Failed to load texture: " << absolutePath << std::endl;
        return player;
    }

    sf::RectangleShape playerShape(sf::Vector2f(33.0f, 17.0f));
    playerShape.setTexture(texture.get());
    playerShape.setTextureRect(sf::IntRect(0, 0, 33, 17));
    registry.add_component(player, core::ge::DrawableComponent{playerShape});
    registry.add_component(player, core::ge::TextureComponent{texture});

    // Animation frames (first row)
    std::vector<sf::IntRect> frames;
    frames.reserve(5);
    for (int i = 0; i < 5; i++) {
        frames.emplace_back(i * 33, 0, 33, 17);
    }
    registry.add_component(player, core::ge::AnimationComponent{frames, 0.1f, 0.0f, 0});

    return player;
}

core::ecs::Entity EntityFactory::createPlayerProjectile(core::ecs::Registry& registry, const sf::Vector2f& startPosition)
{
    core::ecs::Entity projectile = registry.spawn_entity();

    registry.add_component(projectile, core::ge::TransformComponent{startPosition, sf::Vector2f(18.0f, 5.0f), sf::Vector2f(4.0f, 4.0f), 0.0f});
    registry.add_component(projectile, core::ge::CollisionComponent{
        .collisionBoxes = {sf::FloatRect(0.0f, 0.0f, 18.0f, 5.0f)},
        .onCollision = [&](core::ecs::Entity self, core::ecs::Entity other) {
            if (registry.has_component<Enemy>(other)) {
                std::cout << "Projectile hit enemy! Both disappear." << std::endl;
                registry.kill_entity(self);
                registry.kill_entity(other);
            }
        }
    });
    registry.add_component(projectile, VelocityComponent{10.0f, 10.0f});
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
    registry.add_component(projectile, core::ge::TextureComponent{texture});

    return projectile;
}

core::ecs::Entity EntityFactory::createEnemy(core::ecs::Registry &registry, const sf::Vector2f &position)
{
    core::ecs::Entity enemy = registry.spawn_entity();

    registry.add_component(enemy, core::ge::TransformComponent{position, sf::Vector2f(33.0f, 36.0f), sf::Vector2f(4.0f, 4.0f), 0.0f});
    registry.add_component(enemy, core::ge::CollisionComponent{
        .collisionBoxes = {sf::FloatRect(0.0f, 0.0f, 33.0f, 36.0f)},
        .onCollision = [&](core::ecs::Entity self, core::ecs::Entity other) {
            if (registry.has_component<Projectile>(other)) {
                std::cout << "Enemy hit by projectile! Both disappear." << std::endl;
                registry.kill_entity(self);
                registry.kill_entity(other);
            }
        }
    });
    registry.add_component(enemy, VelocityComponent{10.0f, 10.0f});
    registry.add_component(enemy, HealthComponent{100});
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

    std::vector<sf::IntRect> frames;
    frames.reserve(8);
    for (int i = 0; i < 5; i++) {
        frames.emplace_back(i * 33, 0, 33, 36);
    }
    registry.add_component(enemy, core::ge::AnimationComponent{frames, 0.1f, 0.0f, 0});

    return enemy;
}
