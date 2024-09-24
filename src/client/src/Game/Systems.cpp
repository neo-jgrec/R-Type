#include "Systems.hpp"
#include "src/Game/Components.hpp"

void Systems::movementSystem(core::ecs::Registry& registry) {
    registry.add_system<TransformComponent, VelocityComponent, InputStateComponent>(
        [](TransformComponent &transform, const VelocityComponent &vel, InputStateComponent &input) {
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

// void Systems::animationSystem(core::ecs::Registry& registry) {
//     registry.add_system<DrawableComponent, AnimationComponent>(
//         [&](DrawableComponent &drawable, AnimationComponent &anim, sf::Time deltaTime) {
//             // Update elapsed time based on delta time
//             anim.elapsedTime += deltaTime;

//             if (anim.elapsedTime >= anim.frameTime) {
//                 anim.currentFrame = (anim.currentFrame + 1) % anim.frames.size();
//                 anim.elapsedTime -= anim.frameTime; // Keep the remainder for the next update
//             }

//             drawable.shape.setTextureRect(anim.frames[anim.currentFrame]);
//         });
// }


