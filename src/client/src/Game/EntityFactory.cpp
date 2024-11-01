#include "EntityFactory.hpp"
#include "Game.hpp"

#include <SFML/Graphics.hpp>

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "../../../game/Components.hpp"
#include "../../../game/CollisionMask.hpp"
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"
#include "Utils/ClientComponents.hpp"

core::ecs::Entity EntityFactory::createBall(Game &game, const sf::Vector2f& position, const sf::Vector2f& velocity)
{
    auto& gameEngine = game.getGameEngine();
    auto& registry = gameEngine.registry;

    core::ecs::Entity ball = registry.spawn_entity();
    registry.add_component(ball, core::ge::TransformComponent{position, {20, 20}, game.getGameScale(), 0});
    registry.add_component(ball, core::ge::DrawableComponent{sf::RectangleShape(sf::Vector2f(20, 20))});
    registry.add_component(ball, core::ge::VelocityComponent{velocity.x, velocity.y});
    registry.add_component(ball, core::ge::PhysicsComponent{
        .mass = 500.0f,
        .elasticity = 0.5f,
        .friction = 0.0f,
        .isStatic = false,
        .acceleration = {10, 10},
        .forces = {10, 10}
    });
    registry.add_component(ball, core::ge::GravityComponent{});

    registry.add_component(ball, core::ge::CollisionComponent{
        BALL,
        {sf::FloatRect(0, 0, 20, 20)},
        {
            {WORLD_BORDER, [&](const core::ecs::Entity self, const core::ecs::Entity) {
                auto vel = registry.get_component<core::ge::VelocityComponent>(self);
                auto transform = registry.get_component<core::ge::TransformComponent>(self);
                auto physics = registry.get_component<core::ge::PhysicsComponent>(self);

                const auto windowSize = game.getGameEngine().window.getSize();
                const float offset = 0.1f;
                const float effectiveHeight = transform->size.y * transform->scale.y;
                const float effectiveWidth = transform->size.x * transform->scale.x;

                if (transform->position.x <= 0) {
                    transform->position.x = offset;
                    vel->dx = std::abs(vel->dx) * physics->elasticity;
                    physics->forces.x = std::abs(physics->forces.x) * physics->elasticity;
                } else if (transform->position.x >= static_cast<float>(windowSize.x) - effectiveWidth) {
                    transform->position.x = static_cast<float>(windowSize.x) - effectiveWidth - offset;
                    vel->dx = -std::abs(vel->dx) * physics->elasticity;
                    physics->forces.x = -std::abs(physics->forces.x) * physics->elasticity;
                }
                if (transform->position.y <= 20) {
                    transform->position.y = 20 + offset;
                    vel->dy = std::abs(vel->dy) * physics->elasticity;
                    physics->forces.y = std::abs(physics->forces.y) * physics->elasticity;
                } else if (transform->position.y >= static_cast<float>(windowSize.y) - effectiveHeight) {
                    transform->position.y = static_cast<float>(windowSize.y) - effectiveHeight - offset;
                    vel->dy = -std::abs(vel->dy) * physics->elasticity;
                    physics->forces.y = -std::abs(physics->forces.y) * physics->elasticity;
                }
            }}
        }
    });

    return ball;
}

core::ecs::Entity EntityFactory::createPlayer(Game &game, const sf::Vector2f& position, uint8_t id, bool self)
{
    auto& gameEngine = game.getGameEngine();
    auto& registry = gameEngine.registry;
    auto& config = game.getConfigManager();

    core::ecs::Entity player = registry.spawn_entity();

    sf::Vector2f playerSize = sf::Vector2f(
        config.getValue<float>("/player/size/x", 99.0f),
        config.getValue<float>("/player/size/y", 51.0f)
    );

    if (self) {
        core::ecs::Entity player_anim = gameEngine.registry.spawn_entity();

        auto texture = gameEngine.assetManager.getTexture("player_anim");

        sf::RectangleShape animShape(sf::Vector2f(33.0f, 35.0f));
        animShape.setTexture(texture.get());
        animShape.setTextureRect(sf::IntRect(0, 0, 33, 35));

        std::vector<sf::IntRect> moveFrames;
        moveFrames.reserve(8);
        for (int i = 0; i < 8; i++)
            moveFrames.emplace_back(i * 33, 0, 33, 35);

        gameEngine.registry.add_component(player_anim, core::ge::AnimationComponent{
            .animations = {
                {core::ge::AnimationState::Moving, moveFrames}
            },
            .frameTime = 0.1f,
            .elapsedTime = 0.0f,
            .currentFrame = 0,
            .loop = false,
            .recurrence_max = 1,
            .recurrence_count = 0,
            .isPlaying = false
        });

        registry.add_component(player, core::ge::VelocityComponent{0, 0});
        registry.add_component(player_anim, core::ge::TransformComponent{position, sf::Vector2f(33.0f * 3, 35.0f * 3), game.getGameScale(), 0.0f});
        registry.add_component(player_anim, core::ge::DrawableComponent{animShape});
        registry.add_component(player_anim, core::ge::TextureComponent{texture});
        registry.add_component(player_anim, PlayerAnim{id});
    }

    registry.add_component(player, core::ge::TransformComponent{position, playerSize, game.getGameScale(), 0.0f});
    if (self) {
        gameEngine.registry.add_component(player, InputStateComponent{});
    }
    gameEngine.registry.add_component(player, core::ge::KeyBinding{gameEngine.keyBindingsConfig});
    gameEngine.registry.add_component(player, HealthComponent{10});
    gameEngine.registry.add_component(player, ScoreComponent{0});
    gameEngine.registry.add_component(player, Player{id, self});
    gameEngine.registry.add_component(player, ShootCounterComponent{0, 0, 0, -1});
    gameEngine.registry.add_component(player, PlayerColorComponent{id});

    auto texture = gameEngine.assetManager.getTexture("player");

    sf::RectangleShape playerShape(playerSize);
    playerShape.setTexture(texture.get());

    int row = id * 17;
    playerShape.setTextureRect(sf::IntRect(0, row, 33, 17));

    registry.add_component(player, core::ge::DrawableComponent{playerShape});
    registry.add_component(player, core::ge::TextureComponent{texture});
    registry.add_component(player, core::ge::DisabledComponent{false});

    std::vector<sf::IntRect> moveFrames;
    moveFrames.reserve(5);
    for (int i = 0; i < 5; i++) {
        moveFrames.emplace_back(i * 33, row, 33, 17);
    }
    std::vector<sf::IntRect> dieFrames;
    dieFrames.reserve(4);
    for (int i = 0; i < 4; i++) {
        dieFrames.emplace_back(i * 33, 5 * 17, 36, 36);
    }
    gameEngine.registry.add_component(player, core::ge::AnimationComponent{
        .animations = {
            {core::ge::AnimationState::Moving, moveFrames},
            {core::ge::AnimationState::Dying, dieFrames}
        },
        .frameTime = 0.1f,
        .elapsedTime = 0.0f,
        .currentFrame = 0,
        .loop = false
    });

    return player;
}

core::ecs::Entity EntityFactory::createPlayerProjectile(Game &game, core::ge::TransformComponent& playerTransform)
{
    auto& gameEngine = game.getGameEngine();
    auto& registry = gameEngine.registry;
    auto& config = game.getConfigManager();

    core::ecs::Entity projectile = registry.spawn_entity();

    sf::Vector2f projectileSize = sf::Vector2f(
        config.getValue<float>("/player/weapons/0/size/x", 72.0f),
        config.getValue<float>("/player/weapons/0/size/y", 20.0f)
    );

    sf::Vector2f projectileSpeed = sf::Vector2f(
        config.getValue<float>("/player/weapons/0/speed/x", 500.0f),
        config.getValue<float>("/player/weapons/0/speed/y", 0.0f)
    );

    int damage = config.getValue<int>("/player/weapons/0/damage", 10);

    sf::Vector2f startPosition = playerTransform.position;

    float playerWidth = playerTransform.size.x * playerTransform.scale.x;
    float playerHeight = playerTransform.size.y * playerTransform.scale.y;
    startPosition.x += playerWidth;
    startPosition.y += (playerHeight / 2.0f) - (projectileSize.y / 2.0f);

    registry.add_component(projectile, core::ge::TransformComponent{startPosition, projectileSize, game.getGameScale(), 0.0f});
    registry.add_component(projectile, core::ge::CollisionComponent{PLAYER_PROJECTILE, {sf::FloatRect(0.0f, 0.0f, 18.0f, 5.0f)}});
    registry.add_component(projectile, core::ge::VelocityComponent{projectileSpeed.x, projectileSpeed.y});
    registry.add_component(projectile, DamageComponent{damage});
    registry.add_component(projectile, Projectile{});

    auto buffer = gameEngine.assetManager.getSound("shooting");

    sf::Sound sound;
    sound.setBuffer(*buffer);
    gameEngine.registry.add_component(projectile, core::ge::SoundComponent{sound, buffer, true, false});

    auto texture = gameEngine.assetManager.getTexture("player_projectile");

    sf::RectangleShape projectileShape(sf::Vector2f(18.0f, 5.0f));
    projectileShape.setTexture(texture.get());
    projectileShape.setTextureRect(sf::IntRect(0, 0, 18, 5));
    registry.add_component(projectile, core::ge::DrawableComponent{projectileShape});
    registry.add_component(projectile, core::ge::TextureComponent{texture});

    return projectile;
}

core::ecs::Entity EntityFactory::createPlayerMissile(Game &game, core::ge::TransformComponent &playerTransform)
{
    auto& gameEngine = game.getGameEngine();
    auto& registry = gameEngine.registry;
    auto& config = game.getConfigManager();

    core::ecs::Entity missile = registry.spawn_entity();

    sf::Vector2f missileSize = sf::Vector2f(
        config.getValue<float>("/player/weapons/1/size/x", 136.0f),
        config.getValue<float>("/player/weapons/1/size/y", 48.0f)
    );

    sf::Vector2f missileSpeed = sf::Vector2f(
        config.getValue<float>("/player/weapons/1/speed/x", 500.0f),
        config.getValue<float>("/player/weapons/1/speed/y", 0.0f)
    );

    int damage = config.getValue<int>("/player/weapons/1/damage", 50);

    int health = config.getValue<int>("/player/weapons/1/health", 100);

    sf::Vector2f startPosition = playerTransform.position;
    float playerWidth = playerTransform.size.x * playerTransform.scale.x;
    float playerHeight = playerTransform.size.y * playerTransform.scale.y;
    startPosition.x += playerWidth;
    startPosition.y += (playerHeight / 2.0f) - ((missileSize.y / 2.0f) * game.getGameScale().y);

    registry.add_component(missile, core::ge::TransformComponent{startPosition, missileSize, game.getGameScale(), 0.0f});
    registry.add_component(missile, core::ge::CollisionComponent{PLAYER_MISSILE, {sf::FloatRect(0.0f, 0.0f, missileSize.x, missileSize.y)}});
    registry.add_component(missile, core::ge::VelocityComponent{missileSpeed.x, missileSpeed.y});
    registry.add_component(missile, DamageComponent{damage});
    registry.add_component(missile, Projectile{});
    registry.add_component(missile, HealthComponent{health});

    auto buffer = gameEngine.assetManager.getSound("missile_sound");

    sf::Sound sound;
    sound.setBuffer(*buffer);
    gameEngine.registry.add_component(missile, core::ge::SoundComponent{sound, buffer, true, false});

    auto texture = gameEngine.assetManager.getTexture("player_missile");

    sf::RectangleShape missileShape(sf::Vector2f(34.5f, 12.0f));
    missileShape.setTexture(texture.get());
    missileShape.setTextureRect(sf::IntRect(0, 0, 34, 12));
    registry.add_component(missile, core::ge::DrawableComponent{missileShape});
    registry.add_component(missile, core::ge::TextureComponent{texture});

    std::vector<sf::IntRect> moveFrames;
    moveFrames.reserve(2);
    for (int i = 0; i < 2; i++) {
        moveFrames.emplace_back(i * 34, 0, 34, 12);
    }
    gameEngine.registry.add_component(missile, core::ge::AnimationComponent{
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

core::ecs::Entity EntityFactory::createEnemy(Game &game, const sf::Vector2f& position, std::uint8_t enemyId)
{
    auto& gameEngine = game.getGameEngine();
    auto& registry = gameEngine.registry;
    auto& config = game.getConfigManager();

    core::ecs::Entity enemy = registry.spawn_entity();

    sf::Vector2f enemySize = sf::Vector2f(
        config.getValue<float>("/enemies/0/size/x", 115.0f),
        config.getValue<float>("/enemies/0/size/y", 126.0f)
    );

    sf::Vector2f enemySpeed = sf::Vector2f(
        config.getValue<float>("/enemies/0/speed/x", 200.0f),
        config.getValue<float>("/enemies/0/speed/y", 0.0f)
    );

    int enemyHealth = config.getValue<int>("/enemies/0/health", 10.0f);
    int enemyDamage = config.getValue<int>("/enemies/0/damage", 10.0f);

    registry.add_component(enemy, core::ge::TransformComponent{position, enemySize, game.getGameScale(), 0.0f});
    registry.add_component(enemy, core::ge::VelocityComponent{-enemySpeed.x, enemySpeed.y});
    registry.add_component(enemy, HealthComponent{enemyHealth});
    registry.add_component(enemy, DamageComponent{enemyDamage});
    registry.add_component(enemy, Enemy{
        .id = enemyId
    });
    gameEngine.registry.add_component(enemy, core::ge::CollisionComponent{ENEMY, {sf::FloatRect(0.0f, 0.0f, enemySize.x, enemySize.y)}, {
        { PLAYER_PROJECTILE, [&](const core::ecs::Entity self, [[maybe_unused]] const core::ecs::Entity other) {
                auto drawable = gameEngine.registry.get_component<core::ge::DrawableComponent>(self);
                drawable->visible = false;
                drawable->timeSinceLastVisible = sf::Time::Zero;
        }}, { PLAYER_MISSILE, [&](const core::ecs::Entity self, [[maybe_unused]] const core::ecs::Entity other) {
                auto drawable = gameEngine.registry.get_component<core::ge::DrawableComponent>(self);
                drawable->visible = false;
                drawable->timeSinceLastVisible = sf::Time::Zero;
        }},
    }});

    auto texture = gameEngine.assetManager.getTexture("enemie1");

    sf::RectangleShape enemyShape(sf::Vector2f(33.0f, 36.0f));
    enemyShape.setTexture(texture.get());
    enemyShape.setTextureRect(sf::IntRect(0, 0, 33, 36));
    registry.add_component(enemy, core::ge::DrawableComponent{enemyShape});
    registry.add_component(enemy, core::ge::TextureComponent{texture});
    std::vector<sf::IntRect> moveFrames;
    moveFrames.reserve(8);
    for (int i = 0; i < 8; i++)
        moveFrames.emplace_back(i * 33, 0, 33, 36);

    std::vector<sf::IntRect> dieFrames;
    dieFrames.reserve(8);
    for (int i = 0; i < 5; i++)
        dieFrames.emplace_back(i * 33, 36, 33, 35);
    gameEngine.registry.add_component(enemy, core::ge::AnimationComponent{
        .animations = {
            {core::ge::AnimationState::Moving, moveFrames},
            {core::ge::AnimationState::Dying, dieFrames}
        },
        .frameTime = 0.1f,
        .elapsedTime = 0.0f,
        .currentFrame = 0,
        .loop = true
    });

    return enemy;
}

core::ecs::Entity EntityFactory::createShooterEnemy(Game &game, const sf::Vector2f& position, std::uint8_t enemyId)
{
    auto& gameEngine = game.getGameEngine();
    auto& registry = gameEngine.registry;
    auto& config = game.getConfigManager();

    core::ecs::Entity enemy = gameEngine.registry.spawn_entity();
    sf::Vector2f enemySize = sf::Vector2f(
        config.getValue<float>("/enemies/1/size/x"),
        config.getValue<float>("/enemies/1/size/y")
    );

    sf::Vector2f enemySpeed = sf::Vector2f(
        config.getValue<float>("/enemies/1/speed/x"),
        config.getValue<float>("/enemies/1/speed/y")
    );

    int enemyHealth = config.getValue<int>("/enemies/1/health");
    int enemyDamage = config.getValue<int>("/enemies/1/damage");

    registry.add_component(enemy, core::ge::TransformComponent{position, enemySize, game.getGameScale(), 0.0f});
    registry.add_component(enemy, core::ge::VelocityComponent{-enemySpeed.x, enemySpeed.y});
    registry.add_component(enemy, HealthComponent{enemyHealth});
    registry.add_component(enemy, DamageComponent{enemyDamage});
    registry.add_component(enemy, Enemy{
        .id = enemyId
    });
    gameEngine.registry.add_component(enemy, core::ge::CollisionComponent{ENEMY, {sf::FloatRect(0.0f, 0.0f, enemySize.x, enemySize.y)}, {
        { PLAYER_PROJECTILE, [&](const core::ecs::Entity self, [[maybe_unused]] const core::ecs::Entity other) {
                auto drawable = gameEngine.registry.get_component<core::ge::DrawableComponent>(self);
                drawable->visible = false;
                drawable->timeSinceLastVisible = sf::Time::Zero;
        }}, { PLAYER_MISSILE, [&](const core::ecs::Entity self, [[maybe_unused]] const core::ecs::Entity other) {
                auto drawable = gameEngine.registry.get_component<core::ge::DrawableComponent>(self);
                drawable->visible = false;
                drawable->timeSinceLastVisible = sf::Time::Zero;
        }},
    }});

    auto texture = gameEngine.assetManager.getTexture("shooter_enemy");

    sf::RectangleShape enemyShape(enemySize);
    enemyShape.setTexture(texture.get());
    enemyShape.setTextureRect(sf::IntRect(0, 0, 33, 22));
    gameEngine.registry.add_component(enemy, core::ge::DrawableComponent{enemyShape});
    gameEngine.registry.add_component(enemy, core::ge::TextureComponent{texture});
    std::vector<sf::IntRect> moveFrames;
    moveFrames.reserve(8);
    for (int i = 0; i < 8; i++)
        moveFrames.emplace_back(i * 33, 0, 33, 22);
    std::vector<sf::IntRect> dieFrames;
    dieFrames.reserve(6);
    for (int i = 0; i < 6; i++)
        dieFrames.emplace_back(i * 33, 22, 33, 22);
    gameEngine.registry.add_component(enemy, core::ge::AnimationComponent{
        .animations = {
            {core::ge::AnimationState::Moving, moveFrames},
            {core::ge::AnimationState::Dying, dieFrames}
        },
        .frameTime = 0.1f,
        .elapsedTime = 0.0f,
        .currentFrame = 0,
        .loop = true
    });
    return enemy;
}

core::ecs::Entity EntityFactory::createGameEventManager(Game &game)
{
    auto& gameEngine = game.getGameEngine();
    auto& registry = gameEngine.registry;

    const core::ecs::Entity eventManager = registry.spawn_entity();

    registry.add_component(eventManager, EventComponent{});

    return eventManager;
}
