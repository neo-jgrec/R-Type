#include "Systems.hpp"

#include <SFML/System/Vector2.hpp>
#include <ostream>

#include "Utils/ClientComponents.hpp"
#include "EntityFactory.hpp"
#include "src/event/Event.hpp"
#include "src/event/EventPool.hpp"
#include "../../../game/Components.hpp"
#include "../../../game/RequestType.hpp"


static std::pair<std::shared_ptr<core::ge::TransformComponent>, std::shared_ptr<core::ge::AnimationComponent>> getPlayerAnimComponents(core::ecs::Registry& registry)
{
    const auto playerAnimEntities = registry.get_entities<PlayerAnim>();
    if (playerAnimEntities.empty())
        return {nullptr, nullptr};

    return {
        registry.get_component<core::ge::TransformComponent>(playerAnimEntities[0]),
        registry.get_component<core::ge::AnimationComponent>(playerAnimEntities[0])
    };
}


namespace Systems {
    void playerInput(Game &game)
    {
        auto &gameEngine = game.getGameEngine();
        auto& registry = gameEngine.registry;
        const auto& config = game.getConfigManager();
        auto &networkingService = game.getNetworkingService();

        static float autoFireTimer = 0.0f;
        static int autoFireCount = 0;

        registry.add_system<core::ge::TransformComponent, core::ge::VelocityComponent, InputStateComponent, ShootCounterComponent, Player, core::ge::AnimationComponent>(
            [&](core::ecs::Entity, core::ge::TransformComponent &transform, core::ge::VelocityComponent &vel, const InputStateComponent &input, ShootCounterComponent &shootCounter, Player &player, core::ge::AnimationComponent &animation) {

                const auto [playerAnimTransform, playerAnim] = getPlayerAnimComponents(registry);
                auto playerSize = sf::Vector2f(
                    config.getValue<float>("/player/size/x", 99.0f),
                    config.getValue<float>("/player/size/y", 51.0f)
                );
                {
                    if (playerAnimTransform && playerAnim) {
                        playerAnimTransform->position = transform.position + sf::Vector2f(playerSize.x, -15.0f);
                        if (input.up) {
                            if (animation.currentFrame == 3) {
                                animation.elapsedTime += gameEngine.delta_t;
                                if (animation.elapsedTime >= animation.frameTime)
                                    animation.currentFrame = 4;
                            } else if (animation.currentFrame == 4) {
                                animation.currentFrame = 3;
                                animation.elapsedTime = 0;
                            }
                        }
                    }
                }

                vel = core::ge::VelocityComponent(
                    config.getValue<float>("/player/speed/x", 350.0f) * static_cast<float>(input.right - input.left),
                    config.getValue<float>("/player/speed/y", 350.0f) * static_cast<float>(input.down - input.up)
                );

                if (game._autoFire) {
                    autoFireTimer += gameEngine.delta_t;
                    if (autoFireTimer >= 0.3f && autoFireCount < 10) {
                        game.addToScene(EntityFactory::createPlayerProjectile(game, transform));
                        networkingService.sendRequest(
                            "127.0.0.1",
                            1111,
                            PlayerShoot,
                            {player.id}
                        );
                        autoFireTimer = 0.0f;
                        autoFireCount += 1;
                    } else if (autoFireTimer >= 0.3 && autoFireCount >= 10) {
                        game.addToScene(EntityFactory::createPlayerMissile(game, transform));
                        networkingService.sendRequest(
                            "127.0.0.1",
                            1111,
                            PlayerShoot,
                            {player.id}
                        );
                        autoFireTimer = 0.0f;
                        autoFireCount = 0;
                    }
                } else {
                    if (input.fire) {
                        shootCounter.notChargingTime = 0.0f;
                        shootCounter.chargeTime += gameEngine.delta_t;
                        if (shootCounter.chargeTime >= 1.0f) {
                            shootCounter.nextShotType = 1;
                        } else {
                            if (shootCounter.chargeTime >= 0.05f) {
                                playerAnim->currentFrame = 0;
                                if (shootCounter.chargeTime >= 0.1f)
                                    playerAnim->currentFrame = 1;
                                if (shootCounter.chargeTime >= 0.2f)
                                    playerAnim->currentFrame = 2;
                                if (shootCounter.chargeTime >= 0.35f)
                                    playerAnim->currentFrame = 3;
                                if (shootCounter.chargeTime >= 0.5f)
                                    playerAnim->currentFrame = 4;
                                if (shootCounter.chargeTime >= 0.65f)
                                    playerAnim->currentFrame = 5;
                                if (shootCounter.chargeTime >= 0.7f)
                                    playerAnim->currentFrame = 6;
                                if (shootCounter.chargeTime >= 0.8f)
                                    playerAnim->currentFrame = 7;
                                if (shootCounter.chargeTime >= 1.0f)
                                    playerAnim->currentFrame = 8;
                            }
                            shootCounter.nextShotType = 0;
                        }
                    } else {
                        if (shootCounter.nextShotType == 0) {
                            game.addToScene(EntityFactory::createPlayerProjectile(game, transform));
                            networkingService.sendRequest(
                                "127.0.0.1",
                                1111,
                                PlayerShoot,
                                {player.id}
                            );
                        }
                        if (shootCounter.nextShotType == 1) {
                            game.addToScene(EntityFactory::createPlayerMissile(game, transform));
                            networkingService.sendRequest(
                                "127.0.0.1",
                                1111,
                                PlayerShoot,
                                {player.id}
                            );
                        }
                        shootCounter.notChargingTime += gameEngine.delta_t;
                        if (shootCounter.notChargingTime > 0.05f)
                            shootCounter.chargeTime = 0.0f;
                        playerAnim->currentFrame = 0;
                        shootCounter.nextShotType = -1;
                    }
                }
            });
    }

    void playerMovement(Game &game)
    {
        auto &registry = game.getGameEngine().registry;
        auto &networkingService = game.getNetworkingService();

        registry.add_system<core::ge::TransformComponent, core::ge::VelocityComponent, Player>(
            [&](core::ecs::Entity, const core::ge::TransformComponent &transform, const core::ge::VelocityComponent &vel, const Player &player) {
                if (vel.dx == 0 && vel.dy == 0)
                    return;

                const auto [x, y] = sf::Vector2u(transform.position);
                networkingService.sendRequest(
                    "127.0.0.1",
                    1111,
                    PlayerMove,
                    {
                        player.id,
                        static_cast<uint8_t>(x >> 24),
                        static_cast<uint8_t>(x >> 16),
                        static_cast<uint8_t>(x >> 8),
                        static_cast<uint8_t>(x),
                        static_cast<uint8_t>(y >> 24),
                        static_cast<uint8_t>(y >> 16),
                        static_cast<uint8_t>(y >> 8),
                        static_cast<uint8_t>(y)
                    }
                );
            });
    }

    void gameEvent(Game &game)
    {
        auto &gameEngine = game.getGameEngine();
        auto &registry = gameEngine.registry;

        registry.add_system<EventComponent>([&](core::ecs::Entity, EventComponent&) {
            for (auto &event : EventPool::getInstance().getAllEvents()) {
                std::cout << event << std::endl;
                switch (event.getType()) {
                    case PlayerConnect: {
                        const auto playerId = std::get<std::uint8_t>(event.getPayload());
                        game.addToScene(EntityFactory::createPlayer(
                            game,
                            gameEngine.window.getView().getSize() / 2.0f,
                            playerId,
                            event.getHeader().packetId == game.getPlayerConnectionHeader().packetId));
                        break;
                    }

                    case PlayerMove: {
                        auto [id, position] = std::get<std::pair<std::uint8_t, sf::Vector2u>>(event.getPayload());
                        for (auto playerEntity : registry.get_entities<Player>()) {

                            const auto playerTransform = registry.get_component<core::ge::TransformComponent>(playerEntity);
                            playerTransform->position = sf::Vector2f(static_cast<float>(position.x), static_cast<float>(position.y));
                        }
                        break;
                    }

                    case PlayerDie: {
                        const auto playerId = std::get<std::uint8_t>(event.getPayload());
                        for (auto playerEntity : registry.get_entities<Player>()) {
                            if (const auto playerComponent = registry.get_component<Player>(playerEntity);
                                playerComponent->id == playerId) {
                                const auto drawableComp = registry.get_component<core::ge::DrawableComponent>(playerEntity);
                                const auto transformComp = registry.get_component<core::ge::TransformComponent>(playerEntity);
                                drawableComp->shape.setSize(sf::Vector2f(36.0f * 3, 36.0f * 3));
                                transformComp->size = sf::Vector2f(36.0f * 3, 36.0f * 3);
                                transformComp->position = sf::Vector2f(transformComp->position.x, transformComp->position.y);
                                registry.remove_component<core::ge::VelocityComponent>(playerEntity);
                                registry.remove_component<core::ge::TransformComponent>(playerEntity);
                                registry.remove_component<core::ge::CollisionComponent>(playerEntity);
                                const auto animComp = registry.get_component<core::ge::AnimationComponent>(playerEntity);
                                animComp->currentState = core::ge::AnimationState::Dying;
                                animComp->currentFrame = 0;
                                animComp->frameTime = 0.2f;
                                animComp->elapsedTime = 0.0f;
                                animComp->recurrence_max = 1;
                                animComp->recurrence_count = 0;
                                animComp->loop = true;
                                animComp->isPlaying = true;
                            }
                        }
                        break;
                    }

                    case PlayerHit: {
                        const auto playerId = std::get<std::uint8_t>(event.getPayload());
                        for (auto playerEntity : registry.get_entities<Player>()) {
                            if (const auto playerComponent = registry.get_component<Player>(playerEntity); playerComponent->id == playerId) {

                                auto drawableComponent = registry.get_component<core::ge::DrawableComponent>(playerEntity);
                                if (!drawableComponent)
                                    continue;

                                drawableComponent->shape.setFillColor(sf::Color(255, 255, 255, 128));

                                auto hitAnim = registry.add_component<HitAnimationComponent>(playerEntity, HitAnimationComponent{
                                    .blinkCount = 0,
                                    .maxBlinks = 3,
                                    .blinkTimer = 0.0f,
                                    .blinkInterval = 0.2f,
                                    .isTransparent = true
                                });
                            }
                        }
                        break;
                    }

                    case RequestType::GameOver: {
                        game.setGameState(Game::GameState::GameOver);
                        break;
                    }

                    case PlayerDisconnect: {
                        const auto playerId = std::get<std::uint8_t>(event.getPayload());
                        auto playerEntity = registry.get_entities<Player>()[playerId];
                        registry.kill_entity(playerEntity);
                        break;
                    }

                    case TileDestroy: {
                        auto tileDestroyPayload = std::get<sf::Vector2u>(event.getPayload());
                        for (auto tileEntity : registry.get_entities<TileComponent>()) {
                            const auto tileComponent = registry.get_component<TileComponent>(tileEntity);
                            if (tileComponent->position == sf::Vector2f(tileDestroyPayload)) {
                                registry.kill_entity(tileEntity);
                            }
                        }
                        break;
                    }

                    case PlayerShoot: {
                        const auto playerShootPayload = std::get<std::uint8_t>(event.getPayload());
                        for (auto playerEntity : registry.get_entities<Player>()) {
                            if (registry.get_component<Player>(playerEntity)->id != playerShootPayload)
                                continue;

                            auto playerTransform = registry.get_component<core::ge::TransformComponent>(playerEntity);
                            game.addToScene(EntityFactory::createPlayerProjectile(game, *playerTransform));
                        }
                        break;
                    }

                    case EnemySpawn: {
                        auto [id, position] = std::get<std::pair<std::uint8_t, sf::Vector2u>>(event.getPayload());
                        game.addToScene(EntityFactory::createEnemy(game, sf::Vector2f(position), id));
                        break;
                    }

                    case EnemyMove: {
                        auto [id, position] = std::get<std::pair<std::uint8_t, sf::Vector2u>>(event.getPayload());
                        for (auto enemyEntity : registry.get_entities<Enemy>()) {
                            if (registry.get_component<Enemy>(enemyEntity)->id == id)
                                return;

                            const auto enemyTransform = registry.get_component<core::ge::TransformComponent>(enemyEntity);
                            if (!enemyTransform)
                                return;
                            enemyTransform->position = sf::Vector2f(static_cast<float>(position.x), static_cast<float>(position.y));
                        }
                        break;
                    }

                    case EnemyDie: {
                        const auto enemyDiePayload = std::get<std::uint8_t>(event.getPayload());
                        for (auto enemyEntity : registry.get_entities<Enemy>()) {
                            if (registry.get_component<Enemy>(enemyEntity)->id != enemyDiePayload)
                                continue;

                            const auto animComp = registry.get_component<core::ge::AnimationComponent>(enemyEntity);
                            registry.remove_component<core::ge::VelocityComponent>(enemyEntity);
                            registry.remove_component<core::ge::TransformComponent>(enemyEntity);
                            registry.remove_component<core::ge::CollisionComponent>(enemyEntity);
                            animComp->currentState = core::ge::AnimationState::Dying;
                            animComp->currentFrame = 0;
                            animComp->frameTime = 0.2f;
                            animComp->elapsedTime = 0.0f;
                            animComp->recurrence_max = 1;
                            animComp->recurrence_count = 0;
                            animComp->isPlaying = true;
                        }
                        break;
                    }

                    case MapScroll: {
                        const auto mapScrollPayload = static_cast<float>(std::get<std::uint32_t>(event.getPayload()));
                        auto &view = registry.get_component<ViewComponent>(registry.get_entities<ViewComponent>()[0])->view;

                        view.move(mapScrollPayload, 0.0f);
                        gameEngine.window.setView(view);
                        break;
                    }

                    default:
                        break;
                }
            }
        });
    }

    void gameView(Game &game)
    {
        auto& gameEngine = game.getGameEngine();
        auto& registry = game.getGameEngine().registry;
        const auto& config = game.getConfigManager();

        registry.add_system<ViewComponent>(
            [&](core::ecs::Entity, ViewComponent& view) {
                if (gameEngine.currentScene != Game::GameState::Playing)
                    return;

                view.view.move(
                    config.getValue<float>("/view/speed/x", 50.0f) * gameEngine.delta_t,
                    config.getValue<float>("/view/speed/y", 50.0f) * gameEngine.delta_t);
                gameEngine.window.setView(view.view);
            });
    }

    void hitAnimation(Game &game)
    {
        auto &gameEngine = game.getGameEngine();
        auto &registry = gameEngine.registry;

        registry.add_system<core::ge::DrawableComponent, HitAnimationComponent>(
            [&](core::ecs::Entity entity, core::ge::DrawableComponent &drawable, HitAnimationComponent &hitAnim) {
                hitAnim.blinkTimer += gameEngine.delta_t;

                if (hitAnim.blinkTimer >= hitAnim.blinkInterval) {
                    hitAnim.blinkTimer = 0.0f;
                    hitAnim.isTransparent = !hitAnim.isTransparent;

                    if (!hitAnim.isTransparent)
                        hitAnim.blinkCount++;

                    sf::Color color = drawable.shape.getFillColor();
                    color.a = hitAnim.isTransparent ? 128 : 255;
                    drawable.shape.setFillColor(color);

                    if (hitAnim.blinkCount >= hitAnim.maxBlinks) {
                        drawable.shape.setFillColor(sf::Color(255, 255, 255, 255));
                        registry.remove_component<HitAnimationComponent>(entity);
                    }
                }
            });
    }
};

