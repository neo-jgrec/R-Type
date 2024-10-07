#ifndef GAMEENGINE_HPP_
#define GAMEENGINE_HPP_

#include "../Registry/Registry.hpp"
#include "./GameEngineComponents.hpp"

#include <iostream>
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
                            anim.currentFrame = (anim.currentFrame + 1) % anim.frames.size();
                            anim.elapsedTime -= anim.frameTime;
                        }

                        drawable.shape.setTextureRect(anim.frames[anim.currentFrame]);
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
                registry.add_system<ge::TransformComponent, ge::CollisionComponent>(
                    [&](const ecs::Entity entity, const ge::TransformComponent &transform, ge::CollisionComponent &collision) {
                        auto &collisionComponents = registry.get_components<ge::CollisionComponent>();
                        auto &transformComponents = registry.get_components<ge::TransformComponent>();

                        for (size_t i = 0; i < collisionComponents.size(); ++i) {
                            if (!collisionComponents[i].has_value() || !transformComponents[i].has_value())
                                continue;

                            ecs::Entity otherEntity{i};
                            if (entity == otherEntity)
                                continue;

                            const auto &otherCollision = *collisionComponents[i];
                            const auto &otherTransform = *transformComponents[i];


                            for (const auto &box : collision.collisionBoxes) {
                                sf::FloatRect rect = {
                                    box.left + transform.position.x,
                                    box.top + transform.position.y,
                                    box.width * transform.scale.x,
                                    box.height * transform.scale.y
                                };

                                for (const auto &otherBox : otherCollision->collisionBoxes) {
                                    sf::FloatRect otherRect = {
                                        otherBox.left + otherTransform->position.x,
                                        otherBox.top + otherTransform->position.y,
                                        otherBox.width * otherTransform->scale.x,
                                        otherBox.height * otherTransform->scale.y
                                    };

                                    if (!rect.intersects(otherRect))
                                        continue;

                                    for (auto &[mask, onCollision] : collision.onCollision) {
                                        if ((mask & otherCollision->collisionMask) == 0)
                                            continue;
                                        onCollision(entity, otherEntity);
                                    }
                                }
                            }
                        }
                    });
            }
    };
}

#endif /* !GAMEENGINE_HPP_ */
