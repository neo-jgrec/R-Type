#include "Systems.hpp"
#include <SFML/System/Vector2.hpp>
#include "src/Game/Components.hpp"
#include "src/Game/EntityFactory.hpp"
#include "src/Game/Globals.hpp"

void Systems::movementSystem(core::ecs::Registry& registry) {
    registry.add_system<TransformComponent, VelocityComponent, InputStateComponent, Player>(
        [&](TransformComponent &transform, const VelocityComponent &vel, InputStateComponent &input, Player&) {
            if (input.up) {
                transform.position.y -= vel.dy;
                input.up = false;
            }
            if (input.down) {
                transform.position.y += vel.dy;
                input.down = false;
            }
            if (input.left) {
                transform.position.x -= vel.dx;
                input.left = false;
            }
            if (input.right) {
                transform.position.x += vel.dx;
                input.right = false;
            }
            if (input.fire) {
                sf::Vector2f projectilePosition = transform.position;

                float playerWidth = transform.size.x * transform.scale.x;
                float playerHeight = transform.size.y * transform.scale.y;

                projectilePosition.x += playerWidth ;

                float projectileHeight = 5.0f;
                projectilePosition.y += (playerHeight / 2.0f) - (projectileHeight / 2.0f);
                EntityFactory::createPlayerProjectile(registry, projectilePosition);
                input.fire = false;
            }
        });
}

void Systems::renderSystem(core::ecs::Registry &registry, sf::RenderWindow &window)
{
    registry.add_system<DrawableComponent>([&window](DrawableComponent &drawable) {;
        window.draw(drawable.shape);
    });
}

void Systems::positionSystem(core::ecs::Registry &registry)
{
    registry.add_system<DrawableComponent, TransformComponent>(
        [](DrawableComponent &drawable, const TransformComponent &transform) {
            drawable.shape.setPosition(transform.position);
            drawable.shape.setSize(transform.size);
            drawable.shape.setRotation(transform.rotation);
            drawable.shape.setScale(transform.scale);
        });
}

void Systems::animationSystem(core::ecs::Registry& registry) {
    registry.add_system<DrawableComponent, AnimationComponent>(
        [&](DrawableComponent &drawable, AnimationComponent &anim) {
            anim.elapsedTime += DELTA_T;

            if (anim.elapsedTime >= anim.frameTime) {
                anim.currentFrame = (anim.currentFrame + 1) % anim.frames.size();
                anim.elapsedTime -= anim.frameTime;
            }

            drawable.shape.setTextureRect(anim.frames[anim.currentFrame]);
        });
}

void Systems::projectileMovementSystem(core::ecs::Registry& registry) {
    registry.add_system<TransformComponent, VelocityComponent, Projectile>(
        [&](TransformComponent &transform, VelocityComponent &velocity, Projectile&) {
            
            transform.position.x += velocity.dx;

            // Si le projectile sort de la fenêtre, il est détruit ??
        });
}



