#ifndef GAMEENGINE_HPP_
#define GAMEENGINE_HPP_

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>
extern "C"
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include "../Registry/Registry.hpp"
#include "./GameEngineComponents.hpp"
#include "MusicManager.hpp"
#include "AssetManager.hpp"
#ifdef GE_USE_SDL
    #include <SDL.h>
#endif
#include <SFML/Main.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/WindowStyle.hpp>

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
    GameEngine(bool initWindow = true)
        : luaState(nullptr),
          cpuEntity(ecs::Entity{}),
          ramEntity(ecs::Entity{}),
          fpsEntity(ecs::Entity{}) {
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
        registry.register_component<core::ge::VelocityComponent>();
        registry.register_component<core::ge::CollisionComponent>();
        registry.register_component<core::ge::TextComponent>();
        registry.register_component<core::ge::TextInputComponent>();
        registry.register_component<core::ge::SliderComponent>();
        registry.register_component<core::ge::DisabledComponent>();
        registry.register_component<core::ge::MetricsComponent>();
        registry.register_component<core::ge::PhysicsComponent>();
        registry.register_component<core::ge::GravityComponent>();

        // Initialize systems
        positionSystem();
        renderSystems();
        animationSystem();
        soundSystem();
        velocitySystem();
        collisionSystem();
        clickableSystem();
        textSystem();
        textInputSystem();
        sliderSystem();
        physicsSystem();

        luaState = luaL_newstate();
        luaL_openlibs(luaState);

        if (!initWindow)
            return;

        this->initWindow({800, 600}, 60, "Game");
    }

    /**
     * @brief Destructor for the GameEngine class. Defaulted.
     */
    ~GameEngine() = default;

    template <typename... Args>
    std::optional<luabridge::LuaRef> run_script(const std::string &path, const std::string &function, Args &&... args)
    {
        if (luaL_dofile(luaState, path.c_str())) {
            std::cerr << "Error loading script: " << lua_tostring(luaState, -1) << std::endl;
            return std::nullopt;
        }

        luabridge::LuaRef luaFunction = luabridge::getGlobal(luaState, function.c_str());
        if (!luaFunction.isFunction()) {
            std::cerr << "Error loading function: " << lua_tostring(luaState, -1) << std::endl;
            return std::nullopt;
        }

        return luaFunction(std::forward<Args>(args)...);
    }

    void run_collision(const uint8_t wantedMask, const ecs::Entity entity)
    {
        for (const auto &collisionComponent = registry.get_component<ge::CollisionComponent>(entity);
            const auto &[mask, onCollision] : collisionComponent->onCollision) {
            if (wantedMask != mask)
                continue;
            onCollision(entity, entity);
            return;
        }
    }

    float delta_t = 0.0f;               ///< Time delta between frames, used for animations and movement.
    core::ecs::Registry registry;       ///< The entity-component system (ECS) registry managing all entities and components.
    MusicManager musicManager;          ///< Manager for background music in the game.
    #ifdef GE_USE_SDL
        SDL_Window *sdlWindow;          ///< The SDL window where the game is drawn.
        SDL_Renderer *renderer;         ///< The SDL renderer for rendering graphics.
        AssetManager assetManager{renderer}; ///< Manager for game assets such as textures and sounds.
    #else
        sf::RenderWindow window;            ///< The SFML render window where the game is drawn.
        AssetManager assetManager;          ///< Manager for game assets such as textures and sounds.
    #endif
    int currentScene = 0;               ///< The currently active scene, represented by an integer.
    sf::Clock clock;                    ///< SFML clock for tracking time in the game loop.
    core::ge::KeyBinding keyBindingsConfig; ///< The key bindings configuration for the game.
    lua_State *luaState;                ///< The Lua state for running Lua scripts.

    #ifdef GE_USE_SDL
        void initWindow(sf::VideoMode size, [[maybe_unused]] int framerateLimit, const std::string& title, SDL_WindowFlags style = SDL_WINDOW_SHOWN)
    #else
        void initWindow(sf::VideoMode size, [[maybe_unused]] int framerateLimit, const std::string& title , sf::Uint32 style = sf::Style::Default)
    #endif
    {
#ifdef GE_USE_SDL
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
            return;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(sdlWindow);
            SDL_Quit();
            return;
        }

        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(sdlWindow);
            SDL_Quit();
            return;
        }

        // we need to precise this because the sdl create a new pointer for the renderer
        // do not remove the line please
        assetManager = AssetManager{renderer};
#else
        // Initialize SFML window
        window.create(size, title, style);
        window.setFramerateLimit(framerateLimit);
        window.setKeyRepeatEnabled(true);
#endif
    }

    float cpuUsage = 0.0f;
    float ramUsage = 0.0f;
    float fps = 0.0f;
    ecs::Entity cpuEntity;
    ecs::Entity ramEntity;
    ecs::Entity fpsEntity;
    std::unordered_map<std::string, ecs::Entity> customMetrics;
    std::unordered_map<std::string, std::function<std::string()>> metricLambdas;

    void addMetrics(const std::string& name, const std::function<std::string()>& valueFunc, bool metric = false)
    {
        ecs::Entity metricEntity = registry.spawn_entity();
        metricLambdas[name] = valueFunc;
        customMetrics[name] = metricEntity;
        assetManager.loadFont("_ARIAL", "assets/Fonts/Arial.ttf");

        #ifdef GE_USE_SDL
            auto font = assetManager.getFont("_ARIAL");
            SDL_Color White = {255, 255, 255, 255};
            SDL_Surface* metricSurface = TTF_RenderText_Solid(font, (name + ": " + valueFunc()).c_str(), White);
            SDL_Texture* metricTexture = SDL_CreateTextureFromSurface(renderer, metricSurface);
            SDL_FreeSurface(metricSurface);

            int yOffset = 10 + 60 * (customMetrics.size() + 2);
            SDL_Rect metricRect = {10, yOffset, 150, 50};

            if (metric)
                registry.add_component<core::ge::DrawableComponent>(metricEntity, core::ge::DrawableComponent{metricRect, metricTexture});
            registry.add_component<core::ge::MetricsComponent>(metricEntity, core::ge::MetricsComponent{});
        #else
            sf::Text metricText;
            metricText.setFont(assetManager.getFont("_ARIAL"));
            metricText.setCharacterSize(24);
            metricText.setFillColor(sf::Color::White);
            metricText.setString(name + ": " + valueFunc());

            if (metric)
                registry.add_component<core::ge::TextComponent>(metricEntity, core::ge::TextComponent{metricText, assetManager.getFont("_ARIAL"), true});
            registry.add_component<core::ge::MetricsComponent>(metricEntity, core::ge::MetricsComponent{});
        #endif
    }

    void initGameMetrics()
    {
        assetManager.loadFont("_ARIAL", "assets/Fonts/Arial.ttf");

        cpuEntity = registry.spawn_entity();
        ramEntity = registry.spawn_entity();
        fpsEntity = registry.spawn_entity();

        [[maybe_unused]] float yOffsetSfml = 0.0f;

        #ifdef GE_USE_SDL
            auto font = assetManager.getFont("_ARIAL");
            if (!font) {
                std::cerr << "Failed to load font for metrics!" << std::endl;
                return;
            }
            SDL_Color White = {255, 255, 255, 255};
            SDL_Surface* cpuSurface = TTF_RenderText_Solid(font, "CPU: 0.0%", White);
            SDL_Surface* ramSurface = TTF_RenderText_Solid(font, "RAM: 0.0%", White);
            SDL_Surface* fpsSurface = TTF_RenderText_Solid(font, "FPS: 0.0", White);
            SDL_Texture* cpuTexture = SDL_CreateTextureFromSurface(renderer, cpuSurface);
            SDL_Texture* ramTexture = SDL_CreateTextureFromSurface(renderer, ramSurface);
            SDL_Texture* fpsTexture = SDL_CreateTextureFromSurface(renderer, fpsSurface);
            SDL_FreeSurface(cpuSurface);
            SDL_FreeSurface(ramSurface);
            SDL_FreeSurface(fpsSurface);

            SDL_Rect cpuRect = {10, 10, 100, 50};
            SDL_Rect ramRect = {10, 70, 100, 50};
            SDL_Rect fpsRect = {10, 130, 100, 50};

            registry.add_component<core::ge::DrawableComponent>(cpuEntity, core::ge::DrawableComponent{cpuRect, cpuTexture});
            registry.add_component<core::ge::DrawableComponent>(ramEntity, core::ge::DrawableComponent{ramRect, ramTexture});
            registry.add_component<core::ge::DrawableComponent>(fpsEntity, core::ge::DrawableComponent{fpsRect, fpsTexture});
            registry.add_component<core::ge::MetricsComponent>(cpuEntity, core::ge::MetricsComponent{});
            registry.add_component<core::ge::MetricsComponent>(ramEntity, core::ge::MetricsComponent{});
            registry.add_component<core::ge::MetricsComponent>(fpsEntity, core::ge::MetricsComponent{});
        #else
            if (assetManager.getFont("arial").getInfo().family.empty()) {
                std::cerr << "Failed to load font for metrics!" << std::endl;
                return;
            }
            sf::Text cpuText;
            cpuText.setFont(assetManager.getFont("arial"));
            cpuText.setCharacterSize(24);
            cpuText.setFillColor(sf::Color::White);
            cpuText.setPosition(0.0f,yOffsetSfml);
            yOffsetSfml += 30.0f;

            sf::Text ramText;
            ramText.setFont(assetManager.getFont("arial"));
            ramText.setCharacterSize(24);
            ramText.setFillColor(sf::Color::White);
            ramText.setPosition(0.0f, yOffsetSfml);
            yOffsetSfml += 30.0f;

            sf::Text fpsText;
            fpsText.setFont(assetManager.getFont("arial"));
            fpsText.setCharacterSize(24);
            fpsText.setFillColor(sf::Color::White);
            fpsText.setPosition(0.0f, yOffsetSfml);
            yOffsetSfml += 30.0f;

            cpuText.setString("CPU: 0.0%");
            ramText.setString("RAM: 0.0%");
            fpsText.setString("FPS: 0.0");

            registry.add_component<core::ge::TextComponent>(cpuEntity, core::ge::TextComponent{cpuText, assetManager.getFont("arial"), true});
            registry.add_component<core::ge::TextComponent>(ramEntity, core::ge::TextComponent{ramText, assetManager.getFont("arial"), true});
            registry.add_component<core::ge::TextComponent>(fpsEntity, core::ge::TextComponent{fpsText, assetManager.getFont("arial"), true});
            registry.add_component<core::ge::MetricsComponent>(cpuEntity, core::ge::MetricsComponent{});
            registry.add_component<core::ge::MetricsComponent>(ramEntity, core::ge::MetricsComponent{});
            registry.add_component<core::ge::MetricsComponent>(fpsEntity, core::ge::MetricsComponent{});
        #endif

        [[maybe_unused]] int yOffset = 10 + 60 * 3;
        for (const auto& [name, entity] : customMetrics) {
            auto valueFunc = metricLambdas[name];

            #ifdef GE_USE_SDL
                auto font = assetManager.getFont("_ARIAL");
                SDL_Color White = {255, 255, 255, 255};
                SDL_Surface* metricSurface = TTF_RenderText_Solid(font, (name + ": " + valueFunc()).c_str(), White);
                SDL_Texture* metricTexture = SDL_CreateTextureFromSurface(renderer, metricSurface);
                SDL_FreeSurface(metricSurface);

                SDL_Rect metricRect = {10, yOffset, 150, 50};
                registry.add_component<core::ge::DrawableComponent>(entity, core::ge::DrawableComponent{metricRect, metricTexture});
            #else
                sf::Text metricText;
                metricText.setFont(assetManager.getFont("arial"));
                metricText.setCharacterSize(24);
                metricText.setFillColor(sf::Color::White);
                metricText.setString(name + ": " + valueFunc());
                metricText.setPosition(0.0f, yOffsetSfml);
                yOffsetSfml += 30.0f;

                registry.add_component<core::ge::TextComponent>(entity, core::ge::TextComponent{metricText, assetManager.getFont("arial"), true});
            #endif
            registry.add_component<core::ge::MetricsComponent>(entity, core::ge::MetricsComponent{});
            yOffset += 60;
        }
    }

    void disableMetrics()
    {
        // Disable predefined metrics
        #ifdef GE_USE_SDL
            if (registry.has_component<core::ge::DrawableComponent>(cpuEntity))
                registry.remove_component<core::ge::DrawableComponent>(cpuEntity);
            if (registry.has_component<core::ge::DrawableComponent>(ramEntity))
                registry.remove_component<core::ge::DrawableComponent>(ramEntity);
            if (registry.has_component<core::ge::DrawableComponent>(fpsEntity))
                registry.remove_component<core::ge::DrawableComponent>(fpsEntity);

        #else
            if (registry.has_component<core::ge::TextComponent>(cpuEntity)) {
                registry.remove_component<core::ge::TextComponent>(cpuEntity);
            }
            if (registry.has_component<core::ge::TextComponent>(ramEntity)) {
                registry.remove_component<core::ge::TextComponent>(ramEntity);
            }
            if (registry.has_component<core::ge::TextComponent>(fpsEntity)) {
                registry.remove_component<core::ge::TextComponent>(fpsEntity);
            }
        #endif
        // Disable custom metrics
        #ifdef GE_USE_SDL
            for (auto& [name, entity] : customMetrics) {
                if (registry.has_component<core::ge::DrawableComponent>(entity))
                    registry.remove_component<core::ge::DrawableComponent>(entity);
            }
        #else
            for (auto& [name, entity] : customMetrics) {
                if (registry.has_component<core::ge::TextComponent>(entity))
                    registry.remove_component<core::ge::TextComponent>(entity);
            }
        #endif
    }

    void updateMetrics(bool onlyFPS = false)
    {
        // TODO: vérifier que ça marche encore avec sdl car déplacer condition si drawable component
        delta_t = clock.restart().asSeconds();
        fps = 1.0f / delta_t;

        #ifdef GE_USE_SDL
            if (!registry.has_component<core::ge::DrawableComponent>(fpsEntity))
                return;
            auto font = assetManager.getFont("_ARIAL");
            SDL_Color White = {255, 255, 255, 255};

            if (onlyFPS) {
                SDL_Surface* fpsSurface = TTF_RenderText_Solid(font, ("FPS: " + std::to_string(fps)).c_str(), White);
                auto drawable = registry.get_component<core::ge::DrawableComponent>(fpsEntity);
                SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, fpsSurface);
                SDL_DestroyTexture(drawable->texture);
                drawable->texture = newTexture;
                SDL_FreeSurface(fpsSurface);
                return;
            }

            cpuUsage = getCPUUsage();
            ramUsage = getRAMUsage();

            SDL_Surface* cpuSurface = TTF_RenderText_Solid(font, ("CPU: " + std::to_string(cpuUsage) + "%").c_str(), White);
            SDL_Surface* ramSurface = TTF_RenderText_Solid(font, ("RAM: " + std::to_string(ramUsage) + "%").c_str(), White);
            SDL_Surface* fpsSurface = TTF_RenderText_Solid(font, ("FPS: " + std::to_string(fps)).c_str(), White);

            auto updateTexture = [&](ecs::Entity entity, SDL_Surface* surface) {
                SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
                auto drawable = registry.get_component<core::ge::DrawableComponent>(entity);
                SDL_DestroyTexture(drawable->texture);
                drawable->texture = newTexture;
                SDL_FreeSurface(surface);
            };

            updateTexture(cpuEntity, cpuSurface);
            updateTexture(ramEntity, ramSurface);
            updateTexture(fpsEntity, fpsSurface);

            for (auto& [name, entity] : customMetrics) {
                auto valueFunc = metricLambdas[name];
                SDL_Surface* surface = TTF_RenderText_Solid(font, (name + ": " + valueFunc()).c_str(), White);
                SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
                auto drawable = registry.get_component<core::ge::DrawableComponent>(entity);
                SDL_DestroyTexture(drawable->texture);
                drawable->texture = newTexture;
                SDL_FreeSurface(surface);
            }
        #else
            auto updateText = [&](ecs::Entity entity, const std::string& text) {
                auto textComp = registry.get_component<core::ge::TextComponent>(entity);
                textComp->text.setString(text);
            };

            if (onlyFPS) {
                updateText(fpsEntity, "FPS: " + std::to_string(fps));
                return;
            }

            cpuUsage = getCPUUsage();
            ramUsage = getRAMUsage();

            updateText(cpuEntity, "CPU: " + std::to_string(cpuUsage) + "%");
            updateText(ramEntity, "RAM: " + std::to_string(ramUsage) + "%");
            updateText(fpsEntity, "FPS: " + std::to_string(fps));

            for (auto& [name, entity] : customMetrics) {
                auto valueFunc = metricLambdas[name];
                updateText(entity, name + ": " + valueFunc());
            }
        #endif
    }

    void reEnableMetrics()
    {
        initGameMetrics();
    }


protected:
    /**
     * @brief Sets up the system for rendering drawable components.
     *
     * This system renders entities' `DrawableComponent` if they belong to the active scene.
     */
    void renderSystems()
    {
        #ifdef GE_USE_SDL
            registry.add_system<core::ge::DrawableComponent>(
                [&renderer = renderer, &currentScene = currentScene](core::ecs::Entity, core::ge::DrawableComponent &drawable) {
                    if (drawable.texture) {
                        SDL_Rect rect = {drawable.shape.x, drawable.shape.y, drawable.shape.w, drawable.shape.h};
                        SDL_RenderCopy(renderer, drawable.texture, nullptr, &rect);
                    }
                });
        #else
            registry.add_system<core::ge::DrawableComponent>(
                [this, &window = window](core::ecs::Entity, core::ge::DrawableComponent &drawable) {
                    if (!drawable.visible) {
                      drawable.timeSinceLastVisible += sf::seconds(delta_t);
                      if (drawable.timeSinceLastVisible.asSeconds() > 0.5f) {
                        drawable.visible = true;
                        drawable.timeSinceLastVisible = sf::Time::Zero;
                      }
                      return;
                    }
                    window.draw(drawable.shape);
                });
        #endif
    }

    /**
     * @brief Initializes the system that handles entity positions, sizes, scales, and rotations.
     *
     * This system updates the `DrawableComponent` of entities based on their `TransformComponent` (position, size, rotation, and scale).
     */
    void positionSystem()
    {
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
    void animationSystem()
    {
        registry.add_system<core::ge::DrawableComponent, core::ge::AnimationComponent>(
            [this]([[maybe_unused]] core::ecs::Entity entity, core::ge::DrawableComponent &drawable, core::ge::AnimationComponent &anim) {
                #ifdef GE_USE_SDL
                    sf::IntRect sfRect = anim.animations[anim.currentState][anim.currentFrame];
                    SDL_Rect rect = {sfRect.left, sfRect.top, sfRect.width, sfRect.height};
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
    void soundSystem()
    {
        registry.add_system<core::ge::SoundComponent>([](core::ecs::Entity, core::ge::SoundComponent &sound) {
            if (sound.playOnce && !sound.isPlaying) {
                sound.sound.play();
                sound.isPlaying = true;
            }
        });
    }

    /**
     * @brief Sets up the velocity system for handling entity movement.
     *
     * This system updates the position of entities based on their velocity components.
     */
    void velocitySystem() {
        registry.add_system<ge::TransformComponent, ge::VelocityComponent>(
            [&](ecs::Entity, ge::TransformComponent &transform, const ge::VelocityComponent &velocity) {
                transform.position.x += velocity.dx * delta_t;
                transform.position.y += velocity.dy * delta_t;
            });
    }

    /**
     * @brief Sets up the collision detection system for handling interactions between entities.
     *
     * This system checks for collisions between entities and triggers their `onCollision` callbacks if they intersect.
     */
    void collisionSystem() {
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

    /**
     * @brief Sets up the clicable interaction system.
     *
     * This system handles user interactions with buttons, including hover and click states, and adjusts their size when hovered or clicked.
     */
    void clickableSystem() {
        #ifdef GE_USE_SDL
            registry.add_system<core::ge::ClickableComponent, core::ge::DrawableComponent, core::ge::TextComponent, core::ge::TransformComponent>(
                [&renderer = renderer, &currentScene = currentScene](core::ecs::Entity, core::ge::ClickableComponent &button, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text, core::ge::TransformComponent &transform) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Rect rect = {drawable.shape.x, drawable.shape.y, drawable.shape.w, drawable.shape.h};
                    SDL_Point mousePoint = {x, y};
                    button.hovered = SDL_PointInRect(&mousePoint, &rect);
                    button.clicked = button.hovered && SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT);

                    if (button.clicked) {
                        button.onClick();
                        drawable.shape.w = transform.size.x * 0.98f;
                        drawable.shape.h = transform.size.y * 0.98f;
                        text.text.w = transform.size.x * 0.98f;
                        text.text.h = transform.size.y * 0.98f;
                    } else if (button.hovered) {
                        drawable.shape.w = transform.size.x * 1.02f;
                        drawable.shape.h = transform.size.y * 1.02f;
                        text.text.w = transform.size.x * 1.02f;
                        text.text.h = transform.size.y * 1.02f;
                    } else {
                        drawable.shape.w = transform.size.x;
                        drawable.shape.h = transform.size.y;
                        text.text.w = transform.size.x;
                        text.text.h = transform.size.y;
                    }
                    SDL_RenderCopy(renderer, text.textTexture, nullptr, &text.text);
                });
        #else
            registry.add_system<core::ge::ClickableComponent, core::ge::DrawableComponent, core::ge::TextComponent, core::ge::TransformComponent>(
                [&window = window](core::ecs::Entity, core::ge::ClickableComponent &button, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text, core::ge::TransformComponent &transform) {

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
    void textSystem()
    {
        #ifdef GE_USE_SDL
            registry.add_system<core::ge::TextComponent>(
                [&renderer = renderer, &currentScene = currentScene](core::ecs::Entity, core::ge::TextComponent &text) {
                    SDL_RenderCopy(renderer, text.textTexture, nullptr, &text.text);
                });
        #else
            registry.add_system<core::ge::TextComponent>(
                [&window = window](core::ecs::Entity, core::ge::TextComponent &text) {

                    const sf::View currentView = window.getView();
                    if (text.isFixed) {
                        window.setView(window.getDefaultView());
                    }
                    if (text.text.getString().isEmpty()) {
                        std::cerr << "Warning: Attempting to draw empty text" << std::endl;
                        return;
                    }
                    text.text.setFont(text.font);
                    window.draw(text.text);
                    if (text.isFixed) {
                        window.setView(currentView);
                    }
                });
        #endif
    }

    /**
     * @brief Initialize text input system.
     */
    void textInputSystem()
    {
        #ifdef GE_USE_SDL
            registry.add_system<core::ge::TextInputComponent, core::ge::DrawableComponent, core::ge::TextComponent>(
                [&renderer = renderer, &currentScene = currentScene](core::ecs::Entity, core::ge::TextInputComponent &textInput, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text) {
                    (void)text;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Rect rect = {drawable.shape.x, drawable.shape.y, drawable.shape.w, drawable.shape.h};
                    SDL_Point mousePoint = {x, y};
                    textInput.isActive = SDL_PointInRect(&mousePoint, &rect) && SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT);
                    if (textInput.isActive) {
                        SDL_StartTextInput();
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderFillRect(renderer, &rect);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer, &rect);
                    } else {
                        SDL_StopTextInput();
                    }
                });
        #else
            registry.add_system<core::ge::TextInputComponent, core::ge::DrawableComponent, core::ge::TextComponent>(
                [&window = window](core::ecs::Entity, core::ge::TextInputComponent &textInput, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text) {
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
        #endif
    }

    /**
     * @brief Initialize slider interaction system.
     */
    void sliderSystem()
    {
        #ifdef GE_USE_SDL
            registry.add_system<core::ge::SliderComponent>(
                [&renderer = renderer, &currentScene = currentScene](core::ecs::Entity, core::ge::SliderComponent &slider) {
                    (void)currentScene;
                    SDL_Rect bar = {static_cast<int>(slider.bar.getPosition().x), static_cast<int>(slider.bar.getPosition().y), static_cast<int>(slider.bar.getSize().x), static_cast<int>(slider.bar.getSize().y)};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &bar);
                    SDL_Rect handle = {static_cast<int>(slider.handle.getPosition().x), static_cast<int>(slider.handle.getPosition().y), static_cast<int>(slider.handle.getRadius() * 2), static_cast<int>(slider.handle.getRadius() * 2)};
                    SDL_RenderFillRect(renderer, &handle);
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Point mousePoint = {x, y};
                    if (SDL_PointInRect(&mousePoint, &handle) && SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        float percentage = (x - slider.bar.getPosition().x) / static_cast<float>(slider.bar.getSize().x);
                        percentage = std::clamp(percentage, 0.0f, 1.0f);
                        slider.currentValue = slider.minValue + (slider.maxValue - slider.minValue) * percentage;
                        slider.handle.setPosition(slider.bar.getPosition().x + percentage * slider.bar.getSize().x, slider.handle.getPosition().y);
                        if (slider.onChange)
                            slider.onChange(slider.currentValue);
                    }
                });
        #else
            registry.add_system<core::ge::SliderComponent>(
                [&window = window](core::ecs::Entity, core::ge::SliderComponent &slider) {
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
    private:
        static float getCPUUsage()
        {
            #ifdef _WIN32
                return 0.0f;
            #else
                const std::string cmd = "ps -A -o %cpu | awk '{s+=$1} END {print s}'";
                FILE *pipe = popen(cmd.c_str(), "r");
                if (!pipe)
                    return 0.0f;
                char buffer[128];
                std::string result;
                while (!feof(pipe)) {
                    if (fgets(buffer, 128, pipe) != NULL)
                        result += buffer;
                }
                pclose(pipe);
                return std::stof(result);
            #endif
        }
        static float getRAMUsage()
        {
            #ifdef _WIN32
                return 0.0f;
            #else
                const std::string cmd = "free | grep Mem | awk '{print $3/$2 * 100.0}'";
                FILE *pipe = popen(cmd.c_str(), "r");
                if (!pipe)
                    return 0.0f;
                char buffer[128];
                std::string result;
                while (!feof(pipe)) {
                    if (fgets(buffer, 128, pipe) != NULL)
                        result += buffer;
                }
                pclose(pipe);
                return std::stof(result);
            #endif
        }
        void physicsSystem() {
            registry.add_system<ge::TransformComponent, ge::VelocityComponent, ge::PhysicsComponent>(
                [&]([[maybe_unused]] ecs::Entity entity, [[maybe_unused]] ge::TransformComponent &transform,
                    ge::VelocityComponent &velocity, ge::PhysicsComponent &physics) {
                    if (physics.isStatic)
                        return;

                    physics.acceleration.x = physics.forces.x / physics.mass;
                    physics.acceleration.y = physics.forces.y / physics.mass;

                    velocity.dx += physics.acceleration.x * delta_t;
                    velocity.dy += physics.acceleration.y * delta_t;

                    velocity.dx *= (1.0f - physics.friction);
                    velocity.dy *= (1.0f - physics.friction);

                    physics.forces = {0.0f, 0.0f};
                });

            registry.add_system<ge::VelocityComponent, ge::PhysicsComponent, ge::GravityComponent>(
                [&](ecs::Entity, [[maybe_unused]] ge::VelocityComponent &velocity,
                    ge::PhysicsComponent &physics, ge::GravityComponent &gravity) {
                    if (physics.isStatic)
                        return;

                    physics.forces.x += gravity.gravity.x * physics.mass;
                    physics.forces.y += gravity.gravity.y * physics.mass;
                });
        }
    };
}

#endif /* !GAMEENGINE_HPP_ */
