#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include "../../../core/ecs/Registry/Registry.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"
#include "Game.hpp"

/**
 * @class EntityFactory
 * @brief A factory class for creating various types of entities such as players, projectiles, enemies, and UI buttons.
 *
 * The EntityFactory class provides static methods to create game entities and assign the necessary components to them.
 * It centralizes entity creation logic, ensuring consistency in how entities are instantiated and configured.
 */
class EntityFactory {
public:
    /**
     * @brief Creates a player entity with the given position, color, and game scale.
     * 
     * The player is assigned various components including a transform, collision, velocity, input handling, health, score, and a color. 
     * It also loads the player sprite and handles animation setup.
     * 
     * @param registry The ECS registry managing the entities.
     * @param position The initial position of the player.
     * @param color An integer representing the player's color for sprite customization.
     * @param game The game instance to manage player-related resources such as color release.
     * @param gameScale The scale factor for the game to adjust entity sizes.
     * @return The created player entity.
     */
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position, int color, Game &game, sf::Vector2f gameScale, std::uint16_t playerId, bool self = false);

    /**
     * @brief Creates a projectile entity for the player, using the player's position and scale.
     *
     * The projectile is assigned components such as transform, collision, velocity, damage, and drawable.
     * A sound effect for firing the projectile is also included.
     *
     * @param registry The ECS registry managing the entities.
     * @param playerTransform The transform component of the player to position the projectile correctly.
     * @param gameScale The scale factor for the game to adjust entity sizes.
     * @return The created projectile entity.
     */
    static core::ecs::Entity createPlayerProjectile(core::ecs::Registry& registry, core::ge::TransformComponent& playerTransform, sf::Vector2f gameScale);

    /**
     * @brief Creates a missile entity for the player, using the player's position and scale.
     *
     * The missile is similar to the projectile but deals more damage and has a different size and sound.
     * Components include transform, collision, velocity, damage, and animation.
     *
     * @param registry The ECS registry managing the entities.
     * @param playerTransform The transform component of the player to position the missile correctly.
     * @param gameScale The scale factor for the game to adjust entity sizes.
     * @return The created missile entity.
     */
    static core::ecs::Entity createPlayerMissile(core::ecs::Registry& registry, core::ge::TransformComponent& playerTransform, sf::Vector2f gameScale);

    /**
     * @brief Creates an enemy entity with the given position and game scale.
     *
     * The enemy is assigned various components such as transform, collision, velocity, health, damage, and animations.
     * It also reacts to player projectiles by reducing health and being destroyed upon death.
     *
     * @param registry The ECS registry managing the entities.
     * @param position The initial position of the enemy.
     * @param gameScale The scale factor for the game to adjust entity sizes.
     * @return The created enemy entity.
     */
    static core::ecs::Entity createEnemy(core::ecs::Registry& registry, const sf::Vector2f& position, sf::Vector2f gameScale, std::uint8_t enemyId);

    /**
     * @brief Creates a button entity for the game's UI.
     *
     * The button is assigned components such as transform, drawable, text, and a callback function for when the button is clicked.
     * It can be used in various game states (e.g., Main Menu).
     *
     * @param registry The ECS registry managing the entities.
     * @param position The position of the button in the UI.
     * @param size The size of the button.
     * @param label The text label displayed on the button.
     * @param onClick A callback function to be invoked when the button is clicked.
     * @param scene The scene in which the button will be active.
     * @return The created button entity.
     */
    static core::ecs::Entity createButton(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void()>& onClick, int scene);
    static core::ecs::Entity createTextInput(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& placeholder, int scene);
    static core::ecs::Entity createSlider(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void(float)>& onChange, int scene, float currentValue);
};

#endif // ENTITY_FACTORY_HPP
