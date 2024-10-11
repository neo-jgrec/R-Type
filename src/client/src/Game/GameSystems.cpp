#include "Game.hpp"
#include "../../../game/Components.hpp"
#include "EntityFactory.hpp"
#include "Utils/ClientComponents.hpp"

sf::Vector2f getViewBounds(const sf::RenderWindow& window);

void Game::inputSystem(core::ecs::Registry& registry)
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, InputStateComponent, ShootCounterComponent>
    ([&](core::ecs::Entity, core::ge::TransformComponent &transform, const VelocityComponent &vel, InputStateComponent &input, ShootCounterComponent &shootCounter) {
            if (input.up)
                transform.position.y -= vel.dy;
            if (input.down)
                transform.position.y += vel.dy;
            if (input.left)
                transform.position.x -= vel.dx;
            if (input.right)
                transform.position.x += vel.dx;
            if (input.fire) {
                if (shootCounter.shotCount >= 6) {
                    EntityFactory::createPlayerMissile(registry, transform, gameScale);

                    shootCounter.shotCount = 0;
                } else {
                    EntityFactory::createPlayerProjectile(registry, transform, gameScale);

                    shootCounter.shotCount++;
                }
                input.fire = false;
            }
            if (transform.position.x < getViewBounds(_gameEngine.window).x) {
                transform.position.x = getViewBounds(_gameEngine.window).x;
            } else if (transform.position.x > getViewBounds(_gameEngine.window).x + _gameEngine.window.getView().getSize().x)
                transform.position.x = getViewBounds(_gameEngine.window).x + _gameEngine.window.getView().getSize().x;
            if (transform.position.y < getViewBounds(_gameEngine.window).y) {
                transform.position.y = getViewBounds(_gameEngine.window).y;
            } else if (transform.position.y > getViewBounds(_gameEngine.window).y + _gameEngine.window.getView().getSize().y)
                transform.position.y = getViewBounds(_gameEngine.window).y + _gameEngine.window.getView().getSize().y;
    });
}

void Game::projectileMovementSystem(core::ecs::Registry& registry) const
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, Projectile>(
        [&](core::ecs::Entity proj, core::ge::TransformComponent &transform, VelocityComponent &velocity, Projectile&) {
            transform.position.x += velocity.dx;

            if (transform.position.x < getViewBounds(_gameEngine.window).x || transform.position.y < getViewBounds(_gameEngine.window).y) {
                registry.kill_entity(proj);
            }
        });
}

void Game::enemyMovementSystem(core::ecs::Registry& registry) const
{
    registry.add_system<core::ge::TransformComponent, VelocityComponent, Enemy>(
        [&](core::ecs::Entity enemy, core::ge::TransformComponent &transform, VelocityComponent &velocity, Enemy&) {
            transform.position.x -= velocity.dx;

            if (transform.position.x < getViewBounds(_gameEngine.window).x || transform.position.y < getViewBounds(_gameEngine.window).y) {
                registry.kill_entity(enemy);
            }
        });
}

void Game::moveWindowViewSystem(core::ecs::Registry& registry)
{
    registry.add_system<ViewComponent, core::ge::SceneComponent>(
        [&](core::ecs::Entity, ViewComponent &viewComponent, const core::ge::SceneComponent& scene) {
            if (scene.sceneName != static_cast<int>(_gameEngine.currentScene))
                return;
            viewComponent.view.move(1.0f, 0.0f);
            _gameEngine.window.setView(viewComponent.view);
        });
}
