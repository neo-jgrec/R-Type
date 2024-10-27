#ifndef GAMEENGINE_HPP_
#define GAMEENGINE_HPP_

#include "../Registry/Registry.hpp"
#include "./GameEngineComponents.hpp"
#include "MusicManager.hpp"
#include "AssetManager.hpp"
#ifdef GE_USE_SDL
    #include <SDL.h>
#else
    #include <SFML/Graphics.hpp>
#endif
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
        registry.register_component<core::ge::TextComponent>();
        registry.register_component<core::ge::SceneComponent>();
        registry.register_component<core::ge::TextInputComponent>();
        registry.register_component<core::ge::SliderComponent>();
        registry.register_component<core::ge::DisabledComponent>();

        // Initialize systems
        positionSystem();
        renderSystems();
        animationSystem();
        soundSystem();
        collisionSystem();
        clickableSystem();
        textSystem();
        textInputSystem();
        sliderSystem();

        if (!initWindow)
            return;

#ifdef GE_USE_SDL
        // Initialize SDL window
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }
        sdlWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
        if (!sdlWindow) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }
#else
        // Initialize SFML window
        window.create(sf::VideoMode(1920, 1080), "Game", sf::Style::Default);
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(true);
#endif
    }

    /**
     * @brief Destructor for the GameEngine class. Defaulted.
     */
    ~GameEngine() = default;

    float delta_t = 0.0f;               ///< Time delta between frames, used for animations and movement.
    core::ecs::Registry registry;       ///< The entity-component system (ECS) registry managing all entities and components.
    MusicManager musicManager;          ///< Manager for background music in the game.
    AssetManager assetManager;         ///< Manager for loading and managing assets.
    #ifdef GE_USE_SDL
        SDL_Window *sdlWindow;          ///< The SDL window where the game is drawn.
        SDL_Renderer *renderer;         ///< The SDL renderer for rendering graphics.
    #else
        sf::RenderWindow window;            ///< The SFML render window where the game is drawn.
    #endif
    int currentScene = 0;               ///< The currently active scene, represented by an integer.
    sf::Clock clock;                    ///< SFML clock for tracking time in the game loop.
    core::ge::KeyBinding keyBindingsConfig; ///< The key bindings configuration for the game.

    void initWindow(sf::VideoMode size, int framerateLimit, const std::string& title , sf::Uint32 style = sf::Style::Default)
    {
#ifdef GE_USE_SDL
        // Initialize SDL window
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }
        sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.width, size.height, style);
        if (!sdlWindow) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }
        renderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(sdlWindow);
            SDL_Quit();
        }
#else
        // Initialize SFML window
        window.create(size, title, style);
        window.setFramerateLimit(framerateLimit);
        window.setKeyRepeatEnabled(true);
#endif
    }

protected:
    /**
     * @brief Sets up the system for rendering drawable components.
     * 
     * This system renders entities' `DrawableComponent` if they belong to the active scene.
     */
    void renderSystems() {
        registry.add_system<core::ge::DrawableComponent, core::ge::SceneComponent, core::ge::DisabledComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::DrawableComponent &drawable, core::ge::SceneComponent &scene, core::ge::DisabledComponent &disabled) {
                if (scene.sceneName != currentScene || disabled.disabled)
                    return;
                #ifdef GE_USE_SDL
                SDL_RenderCopy(renderer, drawable.texture, nullptr, &drawable.shape);
                #else
                window.draw(drawable.shape);
                #endif
            });

        registry.add_system<core::ge::DrawableComponent, core::ge::SceneComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::DrawableComponent &drawable, core::ge::SceneComponent &scene) {
                if (scene.sceneName != currentScene)
                    return;
                #ifdef GE_USE_SDL
                SDL_RenderCopy(renderer, drawable.texture, nullptr, &drawable.shape);
                #else
                window.draw(drawable.shape);
                #endif
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
                #ifdef GE_USE_SDL
                drawable.shape.x = transform.position.x;
                drawable.shape.y = transform.position.y;
                drawable.shape.w = transform.size.x;
                drawable.shape.h = transform.size.y;
                #else
                drawable.shape.setPosition(transform.position);
                drawable.shape.setSize(transform.size);
                drawable.shape.setRotation(transform.rotation);
                drawable.shape.setScale(transform.scale);
                #endif
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
                #ifdef GE_USE_SDL
                    SDL_Rect rect = anim.animations[anim.currentState][anim.currentFrame];
                    SDL_RenderCopy(renderer, drawable.texture, &rect, &drawable.shape);
                #else
                    drawable.shape.setTextureRect(anim.animations[anim.currentState][anim.currentFrame]);
                #endif
                if (!anim.isPlaying)
                    return;
                if (anim.loop) {
                    anim.elapsedTime += delta_t;
                    if (anim.elapsedTime >= anim.frameTime) {
                        anim.currentFrame = (anim.currentFrame + 1) % anim.animations[anim.currentState].size();
                        anim.elapsedTime -= anim.frameTime;
                    }
                    if (anim.recurrence_max > 0) {
                        if (anim.recurrence_count >= anim.recurrence_max) {
                            anim.isPlaying = false;
                            registry.remove_component<core::ge::AnimationComponent>(entity);
                            registry.remove_component<core::ge::DrawableComponent>(entity);
                            registry.kill_entity(entity);
                        }
                        if (anim.currentFrame == anim.animations[anim.currentState].size() - 1)
                            anim.recurrence_count++;
                    }
                }
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
     * @brief Sets up the clicable interaction system.
     * 
     * This system handles user interactions with buttons, including hover and click states, and adjusts their size when hovered or clicked.
     */
    void clickableSystem() {
        #ifdef GE_USE_SDL
        registry.add_system<core::ge::ClickableComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent, core::ge::TransformComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::ClickableComponent &button, core::ge::SceneComponent &scene, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text, core::ge::TransformComponent &transform) {
                if (scene.sceneName != currentScene)
                    return;

                int x, y;
                SDL_GetMouseState(&x, &y);
                sf::Vector2f worldPos = window.mapPixelToCoords({x, y});

                button.hovered = drawable.shape.contains(worldPos.x, worldPos.y);
                button.clicked = button.hovered && SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT);

                if (button.clicked) {
                    button.onClick();
                    drawable.shape.setSize(transform.size * 0.98f);
                    text.text.setScale(sf::Vector2f(0.98f, 0.98f));
                } else if (button.hovered) {
                    drawable.shape.setSize(transform.size * 1.02f);
                    text.text.setScale(sf::Vector2f(1.02f, 1.02f));
                } else {
                    drawable.shape.setSize(transform.size);
                    text.text.setScale(sf::Vector2f(1.0f, 1.0f));
                }
            });
        #else
        registry.add_system<core::ge::ClickableComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent, core::ge::TransformComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::ClickableComponent &button, core::ge::SceneComponent &scene, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text, core::ge::TransformComponent &transform) {
                if (scene.sceneName != currentScene)
                    return;

                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);

                button.hovered = drawable.shape.getGlobalBounds().contains(worldPos);
                button.clicked = button.hovered && sf::Mouse::isButtonPressed(sf::Mouse::Left);

                if (button.clicked) {
                    button.onClick();
                    drawable.shape.setSize(transform.size * 0.98f);
                    text.text.setScale(sf::Vector2f(0.98f, 0.98f));
                } else if (button.hovered) {
                    drawable.shape.setSize(transform.size * 1.02f);
                    text.text.setScale(sf::Vector2f(1.02f, 1.02f));
                } else {
                    drawable.shape.setSize(transform.size);
                    text.text.setScale(sf::Vector2f(1.0f, 1.0f));
                }
            });
        #endif
    }

    /**
     * @brief Initializes the text rendering system.
     * 
     * This system handles the rendering of `TextComponent` entities that belong to the active scene.
     */
    void textSystem() {
        #ifdef GE_USE_SDL
        registry.add_system<core::ge::TextComponent, core::ge::SceneComponent, core::ge::DrawableComponent>(
            [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::TextComponent &text, core::ge::SceneComponent &scene, core::ge::DrawableComponent &drawable) {
                if (scene.sceneName != currentScene)
                    return;

                if (text.text.getString().isEmpty()) {
                    std::cerr << "Warning: Attempting to draw empty text" << std::endl;
                    return;
                }

                text.text.setFont(text.font);
                SDL_Color color = {text.text.getFillColor().r, text.text.getFillColor().g, text.text.getFillColor().b, text.text.getFillColor().a};
                SDL_Surface *surface = TTF_RenderText_Solid(text.font, text.text.getString().toAnsiString().c_str(), color);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_Rect rect = {drawable.shape.x, drawable.shape.y, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, nullptr, &rect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            });
        #else
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
        #endif
    }

    /**
     * @brief Initialize text input system.
     */
    void textInputSystem()
    {
        #ifdef GE_USE_SDL
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
                    if (textInput.isActive) {
                        drawable.shape.setOutlineColor(sf::Color::Cyan);
                    } else {
                        drawable.shape.setOutlineColor(sf::Color::Black);
                    }
                    textInput.text.setFont(textInput.font);
                    window.draw(textInput.text);
                });
        #else
            registry.add_system<core::ge::TextInputComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent>(
                [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::TextInputComponent &textInput, core::ge::SceneComponent &scene, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text) {
                    if (scene.sceneName != currentScene)
                        return;
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        if (drawable.shape.getGlobalBounds().contains(worldPos)) {
                            textInput.isActive = true;
                        } else {
                            textInput.isActive = false;
                        }
                    }
                    if (textInput.isActive) {
                        drawable.shape.setOutlineColor(sf::Color::Cyan);
                    } else {
                        drawable.shape.setOutlineColor(sf::Color::Black);
                    }
                    text.text.setFont(textInput.font);
                    window.draw(text.text);
                });
        #endif
    }

    /**
     * @brief Initialize slider interaction system.
     */
    void sliderSystem()
    {
        #ifdef GE_USE_SDL
            registry.add_system<core::ge::SliderComponent, core::ge::SceneComponent, core::ge::DrawableComponent>(
                [&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::SliderComponent &slider, core::ge::SceneComponent &scene, core::ge::DrawableComponent &drawable) {
                    if (scene.sceneName != currentScene)
                        return;
                    (void)drawable;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    sf::Vector2f worldPos = window.mapPixelToCoords({x, y});
                    slider.handle.x = slider.bar.x + (slider.bar.w / 2) - slider.handle.r;
                    SDL_RenderCopy(renderer, drawable.texture, nullptr, &slider.bar);
                    SDL_RenderCopy(renderer, drawable.texture, nullptr, &slider.handle);
                    if (!SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))
                        return;
                    SDL_Rect hitbox = slider.bar;
                    hitbox.h += 40;
                    hitbox.w += 20;
                    hitbox.x -= 15;
                    hitbox.y -= 20;
                    if (!SDL_PointInRect(&worldPos, &hitbox))
                        return;
                    float percentage = (worldPos.x - slider.bar.x) / slider.bar.w;
                    percentage = std::clamp(percentage, 0.0f, 1.0f);
                    slider.currentValue = slider.minValue + (slider.maxValue - slider.minValue) * percentage;
                    slider.handle.x = slider.bar.x + percentage * slider.bar.w - slider.handle.r;
                    if (slider.onChange)
                        slider.onChange(slider.currentValue);
                });
        #else
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
        #endif
    }
    };
}

#endif /* !GAMEENGINE_HPP_ */