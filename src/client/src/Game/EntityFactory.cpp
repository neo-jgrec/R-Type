#include "EntityFactory.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include "src/Game/Components.hpp"

core::ecs::Entity EntityFactory::createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position) {
 
    core::ecs::Entity player = registry.spawn_entity();

    registry.add_component(player, TransformComponent{position, sf::Vector2f(33.0f, 17.0f), sf::Vector2f(4.0f, 4.0f), 0.0f});
    registry.add_component(player, CollisionComponent{.collisionBoxes = {sf::FloatRect(0.0f, 0.0f, 33.0f, 17.0f)}});
    registry.add_component(player, VelocityComponent{10.0f, 10.0f});
    registry.add_component(player, InputStateComponent{});
    registry.add_component(player, KeyBinding{});
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
    registry.add_component(player, DrawableComponent{playerShape});
    registry.add_component(player, TextureComponent{texture});

    // Animation frames (first row)
    std::vector<sf::IntRect> frames;
    frames.reserve(5);
    for (int i = 0; i < 5; i++) {
        frames.emplace_back(i * 33, 0, 33, 17);
    }
    registry.add_component(player, AnimationComponent{frames, 0.1f, 0.0f, 0});

    return player;
}

core::ecs::Entity EntityFactory::createPlayerProjectile(core::ecs::Registry& registry, const sf::Vector2f& startPosition) {
    core::ecs::Entity projectile = registry.spawn_entity();
    
    registry.add_component(projectile, TransformComponent{startPosition, sf::Vector2f(18.0f, 5.0f), sf::Vector2f(4.0f, 4.0f), 0.0f});
    registry.add_component(projectile, CollisionComponent{.collisionBoxes = {sf::FloatRect(0.0f, 0.0f, 18.0f, 5.0f)}});
    registry.add_component(projectile, VelocityComponent{10.0f, 10.0f});
    registry.add_component(projectile, DamageComponent{10});
    registry.add_component(projectile, Projectile{});

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
    registry.add_component(projectile, DrawableComponent{projectileShape});
    registry.add_component(projectile, TextureComponent{texture});

    return projectile;
}
