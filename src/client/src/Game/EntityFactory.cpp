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

core::ecs::Entity EntityFactory::createPlayer(core::GameEngine& gameEngine, ConfigManager& config, const sf::Vector2f& position, int color, Game &game, sf::Vector2f gameScale, std::uint16_t playerId, bool self)
{
    core::ecs::Entity player = gameEngine.registry.spawn_entity();
    sf::Vector2f playerSize = sf::Vector2f(
        config.getValue<float>("/player/size/x"),
        config.getValue<float>("/player/size/y")
    );
    sf::Vector2f playerSpeed = sf::Vector2f(
        config.getValue<float>("/player/speed/x"),
        config.getValue<float>("/player/speed/y")
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

        gameEngine.registry.add_component(player_anim, core::ge::TransformComponent{position, sf::Vector2f(33.0f, 35.0f), gameScale * 3.5f, 0.0f});
        gameEngine.registry.add_component(player_anim, core::ge::DrawableComponent{animShape});
        gameEngine.registry.add_component(player_anim, core::ge::TextureComponent{texture});
        gameEngine.registry.add_component(player_anim, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});
        gameEngine.registry.add_component(player_anim, PlayerAnim{static_cast<std::uint8_t>(playerId)});
    }

    gameEngine.registry.add_component(player, core::ge::TransformComponent{position, playerSize, gameScale, 0.0f});
    gameEngine.registry.add_component(player, core::ge::CollisionComponent{PLAYER, {sf::FloatRect(0.0f, 0.0f, playerSize.x, playerSize.y)}, {
        { ENEMY, [&](const core::ecs::Entity self, [[maybe_unused]] const core::ecs::Entity other) {
                auto disabled = gameEngine.registry.get_component<core::ge::DisabledComponent>(self);
                disabled->disabled = true;
        }}}});
    gameEngine.registry.add_component(player, VelocityComponent{playerSpeed.x, playerSpeed.y});
    if (self) {
        gameEngine.registry.add_component(player, InputStateComponent{});
    }
    gameEngine.registry.add_component(player, core::ge::KeyBinding{});
    gameEngine.registry.add_component(player, HealthComponent{10});
    gameEngine.registry.add_component(player, ScoreComponent{0});
    gameEngine.registry.add_component(player, Player{
        .id = static_cast<uint8_t>(playerId),
        .self = self
    });
    gameEngine.registry.add_component(player, ShootCounterComponent{0, 0, 0, -1});
    gameEngine.registry.add_component(player, PlayerColorComponent{color});

    auto texture = gameEngine.assetManager.getTexture("player");

    sf::RectangleShape playerShape(playerSize);
    playerShape.setTexture(texture.get());

    int row = color * 17;
    playerShape.setTextureRect(sf::IntRect(0, row, 33, 17));

    gameEngine.registry.add_component(player, core::ge::DrawableComponent{playerShape});
    gameEngine.registry.add_component(player, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});
    gameEngine.registry.add_component(player, core::ge::TextureComponent{texture});
    gameEngine.registry.add_component(player, core::ge::DisabledComponent{false});

    std::vector<sf::IntRect> moveFrames;
    moveFrames.reserve(5);
    for (int i = 0; i < 5; i++) {
        moveFrames.emplace_back(i * 33, row, 33, 17);
    }
    gameEngine.registry.add_component(player, core::ge::AnimationComponent{
        .animations = {
            {core::ge::AnimationState::Moving, moveFrames}
        },
        .frameTime = 0.1f,
        .elapsedTime = 0.0f,
        .currentFrame = 0,
        .loop = false
    });

    return player;
}

core::ecs::Entity EntityFactory::createPlayerProjectile(core::GameEngine& gameEngine, ConfigManager& config, core::ge::TransformComponent& playerTransform, sf::Vector2f gameScale)
{
    core::ecs::Entity projectile = gameEngine.registry.spawn_entity();

    sf::Vector2f projectileSize = sf::Vector2f(
        config.getValue<float>("/player/weapons/0/size/x"),
        config.getValue<float>("/player/weapons/0/size/y")
    );

    sf::Vector2f startPosition = playerTransform.position;

    float playerWidth = playerTransform.size.x * playerTransform.scale.x;
    float playerHeight = playerTransform.size.y * playerTransform.scale.y;
    startPosition.x += playerWidth;
    startPosition.y += (playerHeight / 2.0f) - (projectileSize.y / 2.0f);

    gameEngine.registry.add_component(projectile, core::ge::TransformComponent{startPosition, projectileSize, gameScale * 4.0f, 0.0f});
    gameEngine.registry.add_component(projectile, core::ge::CollisionComponent{PLAYER_PROJECTILE, {sf::FloatRect(0.0f, 0.0f, 18.0f, 5.0f)}});
    gameEngine.registry.add_component(projectile, VelocityComponent{20.0f, 20.0f});
    gameEngine.registry.add_component(projectile, DamageComponent{10});
    gameEngine.registry.add_component(projectile, Projectile{});

    auto buffer = gameEngine.assetManager.getSound("shooting");

    sf::Sound sound;
    sound.setBuffer(*buffer);
    gameEngine.registry.add_component(projectile, core::ge::SoundComponent{sound, buffer, true, false});

    auto texture = gameEngine.assetManager.getTexture("player_projectile");

    sf::RectangleShape projectileShape(sf::Vector2f(18.0f, 5.0f));
    projectileShape.setTexture(texture.get());
    projectileShape.setTextureRect(sf::IntRect(0, 0, 18, 5));
    gameEngine.registry.add_component(projectile, core::ge::DrawableComponent{projectileShape});
    gameEngine.registry.add_component(projectile, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});
    gameEngine.registry.add_component(projectile, core::ge::TextureComponent{texture});

    return projectile;
}

core::ecs::Entity EntityFactory::createPlayerMissile(core::GameEngine& gameEngine, ConfigManager& config, core::ge::TransformComponent &playerTransform, sf::Vector2f gameScale)
{
    core::ecs::Entity missile = gameEngine.registry.spawn_entity();

    sf::Vector2f missileSize(34.5f, 12.0f);
    sf::Vector2f scale = gameScale * 5.0f;
    sf::Vector2f startPosition = playerTransform.position;
    float playerWidth = playerTransform.size.x * playerTransform.scale.x;
    float playerHeight = playerTransform.size.y * playerTransform.scale.y;
    startPosition.x += playerWidth;
    startPosition.y += (playerHeight / 2.0f) - ((missileSize.y / 2.0f) * scale.y);


    gameEngine.registry.add_component(missile, core::ge::TransformComponent{startPosition, missileSize, scale, 0.0f});
    gameEngine.registry.add_component(missile, core::ge::CollisionComponent{PLAYER_MISSILE, {sf::FloatRect(0.0f, 0.0f, 34.5f, 12.0f)}});
    gameEngine.registry.add_component(missile, VelocityComponent{20.0f, 20.0f});
    gameEngine.registry.add_component(missile, DamageComponent{20});
    gameEngine.registry.add_component(missile, Projectile{});
    gameEngine.registry.add_component(missile, HealthComponent{50});

    auto buffer = gameEngine.assetManager.getSound("missile_sound");

    sf::Sound sound;
    sound.setBuffer(*buffer);
    gameEngine.registry.add_component(missile, core::ge::SoundComponent{sound, buffer, true, false});

    auto texture = gameEngine.assetManager.getTexture("player_missile");

    sf::RectangleShape missileShape(sf::Vector2f(34.5f, 12.0f));
    missileShape.setTexture(texture.get());
    missileShape.setTextureRect(sf::IntRect(0, 0, 34, 12));
    gameEngine.registry.add_component(missile, core::ge::DrawableComponent{missileShape});
    gameEngine.registry.add_component(missile, core::ge::TextureComponent{texture});
    gameEngine.registry.add_component(missile, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});

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

core::ecs::Entity EntityFactory::createEnemy(core::GameEngine& gameEngine, ConfigManager& config, const sf::Vector2f& position, sf::Vector2f gameScale, std::uint8_t enemyId)
{
    core::ecs::Entity enemy = gameEngine.registry.spawn_entity();

    gameEngine.registry.add_component(enemy, core::ge::TransformComponent{position, sf::Vector2f(33.0f, 36.0f), gameScale * 3.5f, 0.0f});
    gameEngine.registry.add_component(enemy, core::ge::CollisionComponent{ENEMY, {sf::FloatRect(0.0f, 0.0f, 33.0f, 36.0f)}, {
        { PLAYER_PROJECTILE, [&](const core::ecs::Entity self, const core::ecs::Entity other) {
                auto enemyHealth = gameEngine.registry.get_component<HealthComponent>(self);
                auto projDamage = gameEngine.registry.get_component<DamageComponent>(other);
                enemyHealth->health -= projDamage->damage;
                gameEngine.registry.remove_component<core::ge::DrawableComponent>(other);
                if (enemyHealth->health > 0)
                    return;
                auto animComp = gameEngine.registry.get_component<core::ge::AnimationComponent>(self);
                gameEngine.registry.remove_component<VelocityComponent>(self);
                gameEngine.registry.remove_component<core::ge::TransformComponent>(self);
                gameEngine.registry.remove_component<core::ge::CollisionComponent>(self);
                animComp->currentState = core::ge::AnimationState::Dying;
                animComp->currentFrame = 0;
                animComp->frameTime = 0.2f;
                animComp->elapsedTime = 0.0f;
                animComp->recurrence_max = 1;
                animComp->recurrence_count = 0;
                animComp->isPlaying = true;
        }}, { PLAYER_MISSILE, [&](const core::ecs::Entity self, const core::ecs::Entity other) {
                auto enemyHealth = gameEngine.registry.get_component<HealthComponent>(self);
                auto missileDamage = gameEngine.registry.get_component<DamageComponent>(other);
                enemyHealth->health -= missileDamage->damage;
                gameEngine.registry.remove_component<core::ge::DrawableComponent>(other);
                if (enemyHealth->health > 0)
                    return;
                auto animComp = gameEngine.registry.get_component<core::ge::AnimationComponent>(self);
                gameEngine.registry.remove_component<VelocityComponent>(self);
                gameEngine.registry.remove_component<core::ge::TransformComponent>(self);
                gameEngine.registry.remove_component<core::ge::CollisionComponent>(self);
                animComp->currentState = core::ge::AnimationState::Dying;
                animComp->currentFrame = 0;
                animComp->frameTime = 0.2f;
                animComp->elapsedTime = 0.0f;
                animComp->recurrence_max = 1;
                animComp->recurrence_count = 0;
                animComp->isPlaying = true;
        }},
    }});
    gameEngine.registry.add_component(enemy, VelocityComponent{2.0f, 2.0f});
    gameEngine.registry.add_component(enemy, HealthComponent{10});
    gameEngine.registry.add_component(enemy, DamageComponent{10});
    gameEngine.registry.add_component(enemy, Enemy{
        .id = enemyId
    });

    auto texture = gameEngine.assetManager.getTexture("enemie1");

    sf::RectangleShape enemyShape(sf::Vector2f(33.0f, 36.0f));
    enemyShape.setTexture(texture.get());
    enemyShape.setTextureRect(sf::IntRect(0, 0, 33, 36));
    gameEngine.registry.add_component(enemy, core::ge::DrawableComponent{enemyShape});
    gameEngine.registry.add_component(enemy, core::ge::TextureComponent{texture});
    gameEngine.registry.add_component(enemy, core::ge::SceneComponent{static_cast<int>(Game::GameState::Playing)});
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

core::ecs::Entity EntityFactory::createButton(core::GameEngine& gameEngine, ConfigManager& config, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void()>& onClick, int scene)
{
    core::ecs::Entity button = gameEngine.registry.spawn_entity();

    sf::RectangleShape shape(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);

    auto font = gameEngine.assetManager.getFont("arial");

    sf::Text text;
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);

    gameEngine.registry.add_component(button, core::ge::TransformComponent{position, size, sf::Vector2f(1.0f, 1.0f), 0.0f});
    gameEngine.registry.add_component(button, core::ge::DrawableComponent{shape});
    gameEngine.registry.add_component(button, core::ge::TextComponent{text, font});
    gameEngine.registry.add_component(button, core::ge::ClickableComponent{false, false, onClick});
    gameEngine.registry.add_component(button, core::ge::SceneComponent{scene});
    return button;
}

core::ecs::Entity EntityFactory::createTextInput(core::GameEngine& gameEngine, ConfigManager& config, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& title, int scene)
{
    core::ecs::Entity textInput = gameEngine.registry.spawn_entity();

    sf::RectangleShape shape(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);

    auto font = gameEngine.assetManager.getFont("arial");

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString(title);
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(position.x, position.y - 40.0f);

    sf::Text text;
    text.setFont(font);
    text.setString("");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(position.x + 10.0f, position.y + 10.0f);


    gameEngine.registry.add_component(textInput, core::ge::TextComponent{titleText, font});
    gameEngine.registry.add_component(textInput, core::ge::DrawableComponent{shape});
    gameEngine.registry.add_component(textInput, core::ge::TextInputComponent{text, font, false, 0, 20});
    gameEngine.registry.add_component(textInput, core::ge::SceneComponent{scene});

    return textInput;
}

core::ecs::Entity EntityFactory::createSlider(core::GameEngine& gameEngine, ConfigManager& config, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void(float)>& onChange, int scene, float currentValue)
{
    core::ecs::Entity slider = gameEngine.registry.spawn_entity();

    sf::RectangleShape bar(size);
    bar.setPosition(position);
    bar.setFillColor(sf::Color::White);
    bar.setOutlineThickness(2);
    bar.setOutlineColor(sf::Color::Black);

    sf::CircleShape handle(15);
    handle.setPosition(position.x + size.x, position.y + size.y / 2);
    handle.setFillColor(sf::Color::Red);

    auto font = gameEngine.assetManager.getFont("arial");

    sf::Text text;
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(position.x, position.y - 40.0f);

    gameEngine.registry.add_component(slider, core::ge::SliderComponent{0, 100, currentValue, bar, handle, onChange});
    gameEngine.registry.add_component(slider, core::ge::SceneComponent{scene});
    gameEngine.registry.add_component(slider, core::ge::TextComponent{text, font});

    return slider;
}

core::ecs::Entity EntityFactory::createImage(core::GameEngine& gameEngine, ConfigManager& config, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& textureName, int scene)
{
    core::ecs::Entity img = gameEngine.registry.spawn_entity();

    sf::RectangleShape shape(size);
    shape.setPosition(position);

    auto texture = gameEngine.assetManager.getTexture(textureName);

    shape.setTexture(texture.get());

    gameEngine.registry.add_component(img, core::ge::DrawableComponent{shape});
    gameEngine.registry.add_component(img, core::ge::TextureComponent{texture});
    gameEngine.registry.add_component(img, core::ge::SceneComponent{scene});

    return img;
}

