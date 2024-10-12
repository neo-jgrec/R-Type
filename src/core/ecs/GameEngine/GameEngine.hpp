#ifndef GAMEENGINE_HPP_
#define GAMEENGINE_HPP_

#include "../Registry/Registry.hpp"
#include "./GameEngineComponents.hpp"
#include "MusicManager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>

namespace core {
/**
 * @class GameEngine
 * @brief Manages the core game loop, systems, and rendering operations.
 * 
 * The `GameEngine` class is responsible for initializing and managing the game components, such as the entity registry, systems,
 * rendering, and sound. It also manages the SFML window and the scene switching logic.
 */
class GameEngine {
public:
    /**
     * @brief Constructs a GameEngine object and initializes components and systems.
     * 
     * The constructor initializes the component registry and sets up various game systems such as rendering, animations, and collisions.
     * Optionally, it can initialize an SFML window for rendering.
     * 
     * @param initWindow A boolean flag that indicates whether to initialize the SFML window. Default is true.
     */
    GameEngine(bool initWindow = true) {
        // Register all necessary components
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
        registry.register_component<core::ge::TextInputComponent>();
        registry.register_component<core::ge::SliderComponent>();

        // Initialize systems
        positionSystem();
        renderSystems();
        animationSystem();
        soundSystem();
        collisionSystem();
        buttonSystem();
        textSystem();
        textInputSystem();
        sliderSystem();

        if (!initWindow)
            return;

        // Initialize window if required
        window.create(sf::VideoMode(1920, 1080), "Game", sf::Style::Default);
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(true);
    }

    /**
     * @brief Destructor for the GameEngine class. Defaulted.
     */
    ~GameEngine() = default;

    float delta_t = 0.0f;               ///< Time delta between frames, used for animations and movement.
    core::ecs::Registry registry;       ///< The entity-component system (ECS) registry managing all entities and components.
    MusicManager musicManager;          ///< Manager for background music in the game.
    sf::RenderWindow window;            ///< The SFML render window where the game is drawn.
    int currentScene = 0;               ///< The currently active scene, represented by an integer.
    sf::Clock clock;                    ///< SFML clock for tracking time in the game loop.

protected:
    /**
     * @brief Sets up the system for rendering drawable components.
     * 
     * This system renders entities' `DrawableComponent` if they belong to the active scene.
     */
    void renderSystems() {
        registry.add_system<core::ge::DrawableComponent, core::ge::SceneComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::DrawableComponent &drawable, core::ge::SceneComponent &scene) {
                if (scene.sceneName != currentScene)
                    return;
                window.draw(drawable.shape);
            });
    }

    /**
     * @brief Initializes the system that handles entity positions, sizes, scales, and rotations.
     * 
     * This system updates the `DrawableComponent` of entities based on their `TransformComponent` (position, size, rotation, and scale).
     */
    void positionSystem() {
        registry.add_system<core::ge::DrawableComponent, core::ge::TransformComponent>(
            []([[maybe_unused]] core::ecs::Entity entity, core::ge::DrawableComponent &drawable, const core::ge::TransformComponent &transform) {
                drawable.shape.setPosition(transform.position);
                drawable.shape.setSize(transform.size);
                drawable.shape.setRotation(transform.rotation);
                drawable.shape.setScale(transform.scale);
            });
    }

    /**
     * @brief Sets up the system for handling animations.
     * 
     * This system updates the current frame of an entity's animation based on the elapsed time and frame duration.
     */
    void animationSystem() {
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

    /**
     * @brief Initializes the sound system for playing sound effects.
     * 
     * This system ensures that sound effects are played once when triggered, and sets `isPlaying` to prevent repeated playback.
     */
    void soundSystem() {
        registry.add_system<core::ge::SoundComponent>([](core::ecs::Entity, core::ge::SoundComponent &sound) {
            if (sound.playOnce && !sound.isPlaying) {
                sound.sound.play();
                sound.isPlaying = true;
            }
        });
    }

    /**
     * @brief Sets up the collision detection system for handling interactions between entities.
     * 
     * This system checks for collisions between entities and triggers their `onCollision` callbacks if they intersect.
     */
    void collisionSystem() {
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

    /**
     * @brief Sets up the button interaction system.
     * 
     * This system handles user interactions with buttons, including hover and click states, and adjusts their size when hovered or clicked.
     */
    void buttonSystem() {
        registry.add_system<core::ge::ButtonComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::ButtonComponent &button, core::ge::SceneComponent &scene, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text) {
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

    /**
     * @brief Initializes the text rendering system.
     * 
     * This system handles the rendering of `TextComponent` entities that belong to the active scene.
     */
    void textSystem() {
        registry.add_system<core::ge::TextComponent, core::ge::SceneComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::TextComponent &text, core::ge::SceneComponent &scene) {
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

    /**
     * @brief Initialize text input system.
     */
    void textInputSystem()
    {
        registry.add_system<core::ge::TextInputComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::TextInputComponent &textInput, core::ge::SceneComponent &scene, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text) {
                if (scene.sceneName != currentScene)
                    return;
                (void)text;
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (drawable.shape.getGlobalBounds().contains(worldPos)) {
                        textInput.isActive = true;
                    } else {
                        textInput.isActive = false;
                    }
                }
            });
    }

    /**
     * @brief Initialize slider interaction system.
     */
    void sliderSystem()
    {
        registry.add_system<core::ge::SliderComponent, core::ge::SceneComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::SliderComponent &slider, core::ge::SceneComponent &scene) {
                if (scene.sceneName != currentScene)
                    return;
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);
                slider.handle.setPosition(
                    slider.handle.getPosition().x,
                    slider.bar.getPosition().y + (slider.bar.getSize().y / 2) - (slider.handle.getRadius())
                );
                window.draw(slider.bar);
                window.draw(slider.handle);
                if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    return;
                sf::FloatRect hitbox = slider.bar.getGlobalBounds();
                hitbox.height += 40;
                hitbox.width += 20;
                hitbox.left -= 15;
                hitbox.top -= 20;
                if (!hitbox.contains(worldPos))
                    return;
                float percentage = (worldPos.x - slider.bar.getPosition().x) / slider.bar.getSize().x;
                percentage = std::clamp(percentage, 0.0f, 1.0f);
                slider.currentValue = slider.minValue + (slider.maxValue - slider.minValue) * percentage;
                slider.handle.setPosition(
                    slider.bar.getPosition().x + percentage * slider.bar.getSize().x - slider.handle.getRadius(),
                    slider.bar.getPosition().y + slider.bar.getSize().y / 2
                );
                if (slider.onChange)
                    slider.onChange(slider.currentValue);
            });
    }
    };
}

#endif /* !GAMEENGINE_HPP_ */