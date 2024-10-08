#ifndef GAMEENGINE_HPP_
#define GAMEENGINE_HPP_

#include "../Registry/Registry.hpp"
#include "./GameEngineComponents.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace core {
    class GameEngine {
        public:
            GameEngine() : window(sf::VideoMode(800, 600), "R-Type")
            {
                window.setFramerateLimit(60);
                window.setKeyRepeatEnabled(true);

                registry.register_component<core::ge::TransformComponent>();
                registry.register_component<core::ge::DrawableComponent>();
                registry.register_component<core::ge::KeyBinding>();
                registry.register_component<core::ge::ClockComponent>();
                registry.register_component<core::ge::AnimationComponent>();
                registry.register_component<core::ge::TextureComponent>();
                registry.register_component<core::ge::SoundComponent>();
                registry.register_component<core::ge::MusicComponent>();
                registry.register_component<core::ge::ClickableComponent>();
                registry.register_component<core::ge::ColorComponent>();
                registry.register_component<core::ge::CollisionComponent>();

                positionSystem();
                renderSystems();
                animationSystem();
                soundSystem();
                collisionSystem();
            }

            ~GameEngine() = default;

            float delta_t = 0.0f;
            core::ecs::Registry registry;
            sf::RenderWindow window;
            sf::Clock clock;

        protected:
            void renderSystems()
            {
                registry.add_system<core::ge::DrawableComponent>([&window = window](core::ecs::Entity, core::ge::DrawableComponent &drawable) {
                    window.draw(drawable.shape);
                });
            }

            void positionSystem()
            {
                registry.add_system<core::ge::DrawableComponent, core::ge::TransformComponent>(
                    []([[maybe_unused]] core::ecs::Entity entity, core::ge::DrawableComponent &drawable, const core::ge::TransformComponent &transform) {
                        drawable.shape.setPosition(transform.position);
                        drawable.shape.setSize(transform.size);
                        drawable.shape.setRotation(transform.rotation);
                        drawable.shape.setScale(transform.scale);
                    });
            }

            void animationSystem()
            {
                registry.add_system<core::ge::DrawableComponent, core::ge::AnimationComponent>(
                    [this]([[maybe_unused]] core::ecs::Entity entity, core::ge::DrawableComponent &drawable, core::ge::AnimationComponent &anim) {
                        anim.elapsedTime += delta_t;

                        if (anim.elapsedTime >= anim.frameTime) {
                            anim.currentFrame = (anim.currentFrame + 1) % anim.animations[anim.currentState].size();
                            anim.elapsedTime -= anim.frameTime;
                        }

                        drawable.shape.setTextureRect(anim.animations[anim.currentState][anim.currentFrame]);
                    });
            }

            void soundSystem()
            {
                registry.add_system<core::ge::SoundComponent>([](core::ecs::Entity, core::ge::SoundComponent &sound) {
                    if (sound.playOnce && !sound.isPlaying) {
                        sound.sound.play();
                        sound.isPlaying = true;
                    }
                });
            }

            void collisionSystem()
            {
                registry.add_system<core::ge::TransformComponent, core::ge::CollisionComponent>(
                    [&](core::ecs::Entity entity, core::ge::TransformComponent &transform1, core::ge::CollisionComponent &collision1) {
                        auto &collisionComponents = registry.get_components<core::ge::CollisionComponent>();
                        auto &transformComponents = registry.get_components<core::ge::TransformComponent>();

                        for (size_t i = 0; i < collisionComponents.size(); ++i) {
                            if (!collisionComponents[i].has_value() || !transformComponents[i].has_value())
                                continue;

                            core::ecs::Entity entity2{i};
                            auto &collision2 = *collisionComponents[i];
                            auto &transform2 = *transformComponents[i];

                            if (entity == entity2)
                                continue;

                            for (const auto &box1 : collision1.collisionBoxes) {
                                sf::FloatRect rect1 = box1;
                                rect1.left += transform1.position.x;
                                rect1.top += transform1.position.y;
                                rect1.width *= transform1.scale.x;
                                rect1.height *= transform1.scale.y;

                                for (const auto &box2 : collision2->collisionBoxes) {
                                    sf::FloatRect rect2 = box2;
                                    rect2.left += transform2->position.x;
                                    rect2.top += transform2->position.y;
                                    rect2.width *= transform2->scale.x;
                                    rect2.height *= transform2->scale.y;

                                    if (rect1.intersects(rect2)) {
                                        if (collision1.onCollision) {
                                            collision1.onCollision(entity, entity2);
                                        }
                                        if (collision2->onCollision) {
                                            collision2->onCollision(entity2, entity);
                                        }
                                    }
                                }
                            }
                        }
                    });
            }
    };
}

#endif /* !GAMEENGINE_HPP_ */
