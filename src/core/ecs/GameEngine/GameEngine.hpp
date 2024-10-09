#ifndef GAMEENGINE_HPP_
#define GAMEENGINE_HPP_

#include "../Registry/Registry.hpp"
#include "./GameEngineComponents.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include <iostream>

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
                registry.register_component<core::ge::ButtonComponent>();
                registry.register_component<core::ge::TextComponent>();
                registry.register_component<core::ge::SceneComponent>();

                positionSystem();
                renderSystems();
                animationSystem();
                soundSystem();
                collisionSystem();
                buttonSystem();
                textSystem();
            }

            ~GameEngine() = default;

            float delta_t = 0.0f;
            core::ecs::Registry registry;
            sf::RenderWindow window;
            int currentScene = 0;
            sf::Clock clock;

        protected:
            void renderSystems()
            {
                registry.add_system<core::ge::DrawableComponent, core::ge::SceneComponent>([&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::DrawableComponent &drawable, core::ge::SceneComponent &scene) {
                    if (scene.sceneName != currentScene)
                        return;
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
                registry.add_system<ge::TransformComponent, ge::CollisionComponent, ge::SceneComponent>(
                    [&](const ecs::Entity entity, const ge::TransformComponent &transform, ge::CollisionComponent &collision, ge::SceneComponent &scene) {
                        if (scene.sceneName != currentScene)
                            return;
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

            void buttonSystem()
            {
                registry.add_system<core::ge::ButtonComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent>([&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::ButtonComponent &button, core::ge::SceneComponent &scene, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text) {
                    if (scene.sceneName != currentScene)
                        return;
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);

                    button.isHovered = button.shape.getGlobalBounds().contains(worldPos);
                    button.isPressed = button.isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left);

                    if (button.isPressed) {
                        button.onClick();
                        drawable.shape.setSize(button.shape.getSize() * 0.98f);
                        text.text.setScale(sf::Vector2f(0.98f, 0.98f));
                    } else if (button.isHovered) {
                        drawable.shape.setSize(button.shape.getSize() * 1.02f);
                        text.text.setScale(sf::Vector2f(1.02f, 1.02f));
                    } else {
                        drawable.shape.setSize(button.shape.getSize());
                        text.text.setScale(sf::Vector2f(1.0f, 1.0f));
                    }
                });
            }

            void textSystem()
            {
                registry.add_system<core::ge::TextComponent, core::ge::SceneComponent>([&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::TextComponent &text, core::ge::SceneComponent &scene) {
                    if (scene.sceneName != currentScene)
                        return;
                    if (text.text.getString().isEmpty()) {
                        std::cerr << "Warning: Attempting to draw empty text" << std::endl;
                        return;
                    }
                    text.text.setFont(text.font);
                    window.draw(text.text);
                });
            }
    };
}

#endif /* !GAMEENGINE_HPP_ */
