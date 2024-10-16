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
    try {
        const std::vector payload = {
            static_cast<uint8_t>(playerId),
            static_cast<uint8_t>(static_cast<int>(position.x) >> 24),
            static_cast<uint8_t>(static_cast<int>(position.x) >> 16),
            static_cast<uint8_t>(static_cast<int>(position.x) >> 8),
            static_cast<uint8_t>(static_cast<int>(position.x)),
            static_cast<uint8_t>(static_cast<int>(position.y) >> 24),
            static_cast<uint8_t>(static_cast<int>(position.y) >> 16),
            static_cast<uint8_t>(static_cast<int>(position.y) >> 8),
            static_cast<uint8_t>(static_cast<int>(position.y))
        };

        networkingService.sendRequest(
            "127.0.0.1",
            1111,
            PlayerMove,
            payload
        );
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Game::inputSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, core::ge::VelocityComponent, InputStateComponent, ShootCounterComponent, Player>(
        [&](core::ecs::Entity, core::ge::TransformComponent &transform, core::ge::VelocityComponent &vel, InputStateComponent &input, ShootCounterComponent &shootCounter, Player &player) {
            vel = core::ge::VelocityComponent{
            static_cast<float>(input.left * -200 + input.right * 200),
            static_cast<float>(input.up * -200 + input.down * 200)
            };

            if (input.fire) {
                if (shootCounter.shotCount >= 6) {
                    EntityFactory::createPlayerMissile(registry, transform, gameScale);

                    shootCounter.shotCount = 0;
                } else {
                    EntityFactory::createPlayerProjectile(registry, transform, gameScale);

                    shootCounter.shotCount++;
                }
                input.fire = false;
                networkingService.sendRequest(
                    "127.0.0.1",
                    1111,
                    PlayerShoot,
                    {player.id}
                );
            }

            if (transform.position.x < getViewBounds(_gameEngine.window).x) {
                transform.position.x = getViewBounds(_gameEngine.window).x;
                sendPayloadMove(networkingService, transform.position, player.id);
            } else if (transform.position.x > getViewBounds(_gameEngine.window).x + _gameEngine.window.getView().getSize().x) {
                transform.position.x = getViewBounds(_gameEngine.window).x + _gameEngine.window.getView().getSize().x;
                sendPayloadMove(networkingService, transform.position, player.id);
            }
            if (transform.position.y < getViewBounds(_gameEngine.window).y) {
                transform.position.y = getViewBounds(_gameEngine.window).y;
                sendPayloadMove(networkingService, transform.position, player.id);
            } else if (transform.position.y > getViewBounds(_gameEngine.window).y + _gameEngine.window.getView().getSize().y) {
                transform.position.y = getViewBounds(_gameEngine.window).y + _gameEngine.window.getView().getSize().y;
                sendPayloadMove(networkingService, transform.position, player.id);
            }
    });
}

void Game::playerMovementSystem(core::ecs::Registry& registry) const
{
    registry.add_system<core::ge::TransformComponent, core::ge::VelocityComponent, Player>(
        [&](core::ecs::Entity, const core::ge::TransformComponent &transform, const core::ge::VelocityComponent &vel, const Player &player) {
            if (vel.dx == 0 || vel.dy == 0)
                return;
            sendPayloadMove(networkingService, transform.position, player.id);
        });
}

void Game::projectileMovementSystem(core::ecs::Registry& registry) const
{
    registry.add_system<core::ge::TransformComponent, Projectile>(
        [&](const core::ecs::Entity entity, const core::ge::TransformComponent &transform, Projectile&) {
            if (transform.position.x < getViewBounds(_gameEngine.window).x || transform.position.y < getViewBounds(_gameEngine.window).y)
                return;
            registry.kill_entity(entity);
        });
}

void Game::enemyMovementSystem(core::ecs::Registry& registry) const
{
    registry.add_system<core::ge::TransformComponent, Enemy>(
        [&](const core::ecs::Entity entity, const core::ge::TransformComponent &transform, Enemy&) {
            if (transform.position.x < getViewBounds(_gameEngine.window).x || transform.position.y < getViewBounds(_gameEngine.window).y)
                return;
            registry.kill_entity(entity);
        });
}

void Game::eventSystem(core::ecs::Registry& registry)
{
    registry.add_system<EventComponent>(
        [&](core::ecs::Entity, EventComponent&) {
            std::vector<Event> allEvents = EventPool::getInstance().getAllEvents();
            for (auto &event : allEvents) {
                try {
                    std::cout << event << "/" << event.getType() << std::endl;
                    RequestType type = event.getType();
                    if (type == RequestType::PlayerConnect) {
                        if (event.getHeader().packetId == playerConnectionHeader.packetId) {
                            auto playerId = std::get<std::uint8_t>(event.getPayload());
                            EntityFactory::createPlayer(registry, sf::Vector2f(_gameEngine.window.getView().getSize().x / 2.0f, _gameEngine.window.getView().getSize().y / 2.0f), playerId, *this, gameScale, playerId, true);
                        } else {
                            auto playerId = std::get<std::uint8_t>(event.getPayload());
                            EntityFactory::createPlayer(registry, sf::Vector2f(_gameEngine.window.getView().getSize().x / 2.0f, _gameEngine.window.getView().getSize().y / 2.0f), playerId, *this, gameScale, playerId, false);
                        }
                    } else if (type == RequestType::PlayerMove) {
                        auto playerMovePayload = std::get<std::pair<std::uint8_t, sf::Vector2u>>(event.getPayload());
                        auto playerEntities = registry.get_entities<Player>();
                        for (auto playerEntity : playerEntities) {
                            auto playerComponent = registry.get_component<Player>(playerEntity);
                            if (playerComponent->id == playerMovePayload.first) {
                                auto playerTransform = registry.get_component<core::ge::TransformComponent>(playerEntity);
                                playerTransform->position = sf::Vector2f(static_cast<float>(playerMovePayload.second.x), static_cast<float>(playerMovePayload.second.y));
                            }
                        }
                    } else if (type == RequestType::PlayerDie) {
                        auto playerId = std::get<std::uint8_t>(event.getPayload());
                        auto playerEntities = registry.get_entities<Player>();
                        for (auto playerEntity : playerEntities) {
                            auto playerComponent = registry.get_component<Player>(playerEntity);
                            if (playerComponent->id == playerId) {
                                registry.kill_entity(playerEntity);
                            }
                        }
                    } else if (type == RequestType::PlayerHit) {
                        auto playerId = std::get<std::uint8_t>(event.getPayload());
                        auto playerEntities = registry.get_entities<Player>();
                        for (auto playerEntity : playerEntities) {
                            auto playerComponent = registry.get_component<Player>(playerEntity);
                            if (playerComponent->id == playerId) {
                                auto healthComponent = registry.get_component<HealthComponent>(playerEntity);
                                healthComponent->health -= 10;
                                if (healthComponent->health <= 0) {
                                    registry.kill_entity(playerEntity);
                                }
                            }
                        }
                        // TODO: Handle player hit
                    } else if (type == RequestType::GameOver) {
                        _gameEngine.currentScene = static_cast<int>(GameState::GameOver);
                    } else if (type == RequestType::PlayerDisconnect) {
                        auto playerId = std::get<std::uint8_t>(event.getPayload());
                        auto playerEntity = registry.get_entities<Player>()[playerId];
                        registry.kill_entity(playerEntity);
                    } else if (type == RequestType::TileDestroy) {
                        auto tileDestroyPayload = std::get<sf::Vector2u>(event.getPayload());
                        auto tileEntities = registry.get_entities<Tile>();
                        for (auto tileEntity : tileEntities) {
                            auto tileComponent = registry.get_component<Tile>(tileEntity);
                            if (tileComponent->position == sf::Vector2f(tileDestroyPayload)) {
                                registry.kill_entity(tileEntity);
                            }
                        }
                    } else if (type == RequestType::PlayerShoot) {
                        auto playerShootPayload = std::get<std::uint8_t>(event.getPayload());
                        auto playerEntities = registry.get_entities<Player>();
                        for (auto playerEntity : playerEntities) {
                            auto playerComponent = registry.get_component<Player>(playerEntity);
                            if (playerComponent->id == playerShootPayload) {
                                auto playerTransform = registry.get_component<core::ge::TransformComponent>(playerEntity);
                                EntityFactory::createPlayerProjectile(registry, *playerTransform, gameScale);
                            }
                        }
                    } else if (type == RequestType::EnemySpawn) {
                        auto enemySpawnPayload = std::get<std::pair<std::uint8_t, sf::Vector2u>>(event.getPayload());
                        EntityFactory::createEnemy(registry, sf::Vector2f(enemySpawnPayload.second), gameScale, enemySpawnPayload.first);
                    } else if (type == RequestType::EnemyMove) {
                        auto enemyMovePayload = std::get<std::pair<std::uint8_t, sf::Vector2u>>(event.getPayload());
                        auto enemyEntities = registry.get_entities<Enemy>();
                        for (auto enemyEntity : enemyEntities) {
                            auto enemyComponent = registry.get_component<Enemy>(enemyEntity);
                            if (enemyComponent->id == enemyMovePayload.first) {
                                auto enemyTransform = registry.get_component<core::ge::TransformComponent>(enemyEntity);
                                enemyTransform->position = sf::Vector2f(static_cast<float>(enemyMovePayload.second.x), static_cast<float>(enemyMovePayload.second.y));
                            }
                        }
                    } else if (type == RequestType::EnemyDie) {
                        auto enemyDiePayload = std::get<std::uint8_t>(event.getPayload());
                        auto enemyEntities = registry.get_entities<Enemy>();
                        for (auto enemyEntity : enemyEntities) {
                            auto enemyComponent = registry.get_component<Enemy>(enemyEntity);
                            if (enemyComponent->id == enemyDiePayload) {
                                registry.kill_entity(enemyEntity);
                            }
                        }
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Error processing event: " << e.what() << std::endl;
                }
            }
        });
}

void Game::viewSystem(core::ecs::Registry& registry)
{
    registry.add_system<ViewComponent, core::ge::SceneComponent>(
        [&](core::ecs::Entity, ViewComponent& view, [[maybe_unused]] core::ge::SceneComponent& scene) {
            if (_gameEngine.currentScene != static_cast<int>(GameState::Playing))
                return;

            float scrollAmount = 20.0f * _gameEngine.delta_t;

            view.view.move(scrollAmount, 0);
            _gameEngine.window.setView(view.view);
        });
}
