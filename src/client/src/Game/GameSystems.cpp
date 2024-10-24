#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include "Game.hpp"
#include "../../../game/Components.hpp"
#include "EntityFactory.hpp"
#include "Utils/ClientComponents.hpp"
#include "src/event/Event.hpp"
#include "src/event/EventPool.hpp"
#include "../../../game/RequestType.hpp"

sf::Vector2f getViewBounds(const sf::RenderWindow& window);

static void sendPayloadMove(NetworkingService& networkingService, const sf::Vector2f& position, uint8_t playerId)
{
    std::cout << "Sending move payload" << std::endl;

    const auto [x, y] = sf::Vector2u(position);
    networkingService.sendRequest(
        "127.0.0.1",
        1111,
        PlayerMove,
        {
            playerId,
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
}

std::pair<std::shared_ptr<core::ge::TransformComponent>, std::shared_ptr<core::ge::AnimationComponent>> getPlayerAnimComponents(core::ecs::Registry& registry)
{
    const auto playerAnimEntities = registry.get_entities<PlayerAnim>();
    if (playerAnimEntities.empty())
        return {nullptr, nullptr};

    return {
        registry.get_component<core::ge::TransformComponent>(playerAnimEntities[0]),
        registry.get_component<core::ge::AnimationComponent>(playerAnimEntities[0])
    };
}

void Game::inputSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, core::ge::VelocityComponent, InputStateComponent, ShootCounterComponent, Player, core::ge::AnimationComponent>(
        [&](core::ecs::Entity, core::ge::TransformComponent &transform, core::ge::VelocityComponent &vel, const InputStateComponent &input, ShootCounterComponent &shootCounter, Player &player, core::ge::AnimationComponent &animation) {

            const auto playerAnim = getPlayerAnimComponents(registry).second;
            {
                const auto playerAnimTransform = getPlayerAnimComponents(registry).first;
                if (playerAnimTransform && playerAnim) {
                    playerAnimTransform->position = transform.position + sf::Vector2f(115.0f, 10.0f);
                    if (input.up) {
                        if (animation.currentFrame == 3) {
                            animation.elapsedTime += _gameEngine.delta_t;
                            if (animation.elapsedTime >= animation.frameTime)
                                animation.currentFrame = 4;
                        } else if (animation.currentFrame == 4) {
                            animation.currentFrame = 3;
                            animation.elapsedTime = 0;
                        }
                    }
                }
            }

            constexpr uint16_t speed = 350;
            vel = core::ge::VelocityComponent{
                static_cast<float>(input.left * -speed + input.right * speed),
                static_cast<float>(input.up * -speed + input.down * speed)
            };

            if (input.fire) {
                shootCounter.notChargingTime = 0.0f;
                shootCounter.chargeTime += _gameEngine.delta_t;
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
                    EntityFactory::createPlayerProjectile(_gameEngine, registry, transform, gameScale);
                    networkingService.sendRequest(
                        "127.0.0.1",
                        1111,
                        PlayerShoot,
                        {player.id}
                    );
                }
                if (shootCounter.nextShotType == 1) {
                    EntityFactory::createPlayerMissile(_gameEngine, registry, transform, gameScale);
                    networkingService.sendRequest(
                        "127.0.0.1",
                        1111,
                        PlayerShoot,
                        {player.id}
                    );
                }
                shootCounter.notChargingTime += _gameEngine.delta_t;
                if (shootCounter.notChargingTime > 0.05f)
                    shootCounter.chargeTime = 0.0f;
                playerAnim->currentFrame = 0;
                shootCounter.nextShotType = -1;
            }
        });
}

void Game::playerMovementSystem(core::ecs::Registry& registry) const
{
    registry.add_system<core::ge::TransformComponent, core::ge::VelocityComponent, Player>(
        [&](core::ecs::Entity, const core::ge::TransformComponent &transform, const core::ge::VelocityComponent &vel, const Player &player) {
            if (vel.dx == 0 && vel.dy == 0)
                return;
            sendPayloadMove(networkingService, transform.position, player.id);
        });
}

void Game::projectileMovementSystem(core::ecs::Registry& registry) const
{
    registry.add_system<core::ge::TransformComponent, Projectile>(
        [&](const core::ecs::Entity entity, const core::ge::TransformComponent &transform, Projectile&) {
            if (transform.position.x < getViewBounds(_gameEngine.window).x
                || transform.position.y < getViewBounds(_gameEngine.window).y)
                return;
            registry.remove_component<core::ge::DrawableComponent>(entity);
        });
}

void Game::enemyMovementSystem(core::ecs::Registry& registry) const
{
    registry.add_system<core::ge::TransformComponent, Enemy>(
        [&](const core::ecs::Entity entity, const core::ge::TransformComponent &transform, Enemy&) {
            if (transform.position.x < getViewBounds(_gameEngine.window).x
                || transform.position.y < getViewBounds(_gameEngine.window).y)
                return;
            registry.remove_component<core::ge::DrawableComponent>(entity);
        });
}

void Game::eventSystem(core::ecs::Registry& registry)
{
    registry.add_system<EventComponent>([&](core::ecs::Entity, EventComponent&) {
        for (auto &event : EventPool::getInstance().getAllEvents()) {
            std::cout << event << "/" << event.getType() << std::endl;
            switch (event.getType()) {
                case PlayerConnect: {
                    const auto playerId = std::get<std::uint8_t>(event.getPayload());
                    std::cout << "Player " << static_cast<int>(playerId) << " connected" << std::endl;
                    EntityFactory::createPlayer(
                        _gameEngine,
                        registry,
                        _gameEngine.window.getView().getSize() / 2.0f,
                        playerId,
                        *this,
                        gameScale,
                        playerId,
                        event.getHeader().packetId == playerConnectionHeader.packetId);
                    break;
                }

                case PlayerMove: {
                    std::cout << "Player move" << std::endl;
                    auto [id, position] = std::get<std::pair<std::uint8_t, sf::Vector2u>>(event.getPayload());
                    for (auto playerEntity : registry.get_entities<Player>()) {
                        if (const auto playerComponent = registry.get_component<Player>(playerEntity);
                            playerComponent->id == id)
                            return;

                        if (const auto disabledComponent = registry.get_component<core::ge::DisabledComponent>(playerEntity))
                            disabledComponent->disabled = false;

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
                            registry.kill_entity(playerEntity);
                        }
                    }
                    break;
                }

                case PlayerHit: {
                    const auto playerId = std::get<std::uint8_t>(event.getPayload());
                    for (auto playerEntity : registry.get_entities<Player>()) {
                        if (registry.get_component<Player>(playerEntity)->id != playerId)
                            continue;
                        const auto healthComponent = registry.get_component<HealthComponent>(playerEntity);
                        healthComponent->health -= 10;
                        if (healthComponent->health <= 0)
                            registry.kill_entity(playerEntity);
                    }
                    // TODO: Handle player hit
                    break;
                }

                case RequestType::GameOver: {
                    _gameEngine.currentScene = GameState::GameOver;
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
                    for (auto tileEntity : registry.get_entities<Tile>()) {
                        if (const auto tileComponent = registry.get_component<Tile>(tileEntity);
                            tileComponent->position == sf::Vector2f(tileDestroyPayload)) {
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
                        EntityFactory::createPlayerProjectile(_gameEngine, registry, *playerTransform, gameScale);
                    }
                    break;
                }

                case EnemySpawn: {
                    auto [id, position] = std::get<std::pair<std::uint8_t, sf::Vector2u>>(event.getPayload());
                    EntityFactory::createEnemy(_gameEngine, registry, sf::Vector2f(position), gameScale, id);
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
                        if (registry.get_component<Enemy>(enemyEntity)->id == enemyDiePayload)
                            return;

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

                default:
                    break;
            }
        }
    });
}

void Game::viewSystem(core::ecs::Registry& registry)
{
    registry.add_system<ViewComponent>(
        [&](core::ecs::Entity, ViewComponent& view) {
            if (_gameEngine.currentScene != Playing)
                return;

            view.view.move(50 * _gameEngine.delta_t, 0);
            _gameEngine.window.setView(view.view);
        });
}
