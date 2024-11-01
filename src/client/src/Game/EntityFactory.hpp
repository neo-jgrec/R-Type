#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"
#include "Game.hpp"
#include "../../../core/config/ConfigManager.hpp"

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
     * @param position The initial position of the player.
     * @param id An integer representing the player's color for sprite customization.
     * @param game The game instance to manage player-related resources such as color release.
     * @return The created player entity.
     */
    static core::ecs::Entity createPlayer(Game &game, const sf::Vector2f& position, uint8_t id, bool self);

    /**
     * @brief Creates a projectile entity for the player, using the player's position and scale.
     *
     * The projectile is assigned components such as transform, collision, velocity, damage, and drawable.
     * A sound effect for firing the projectile is also included.
     *
     * @param playerTransform The transform component of the player to position the projectile correctly.
     * @return The created projectile entity.
     */
    static core::ecs::Entity createPlayerProjectile(Game &game, core::ge::TransformComponent& playerTransform);

    /**
     * @brief Creates a missile entity for the player, using the player's position and scale.
     *
     * The missile is similar to the projectile but deals more damage and has a different size and sound.
     * Components include transform, collision, velocity, damage, and animation.
     *
     * @param playerTransform The transform component of the player to position the missile correctly.
     * @return The created missile entity.
     */
    static core::ecs::Entity createPlayerMissile(Game &game, core::ge::TransformComponent& playerTransform);

    /**
     * @brief Creates an enemy entity with the given position and game scale.
     *
     * The enemy is assigned various components such as transform, collision, velocity, health, damage, and animations.
     * It also reacts to player projectiles by reducing health and being destroyed upon death.
     *
     * @param position The initial position of the enemy.
     * @return The created enemy entity.
     */
    static core::ecs::Entity createEnemy(Game &game, const sf::Vector2f& position, std::uint8_t enemyId);

    /**
     * @brief Creates a shooter enemy entity with the given position and ID.
     *
     * The shooter enemy is a specialized type of enemy that can fire projectiles at the player.
     * It is assigned components for transform, collision, velocity, health, damage, and animations.
     * Unlike regular enemies, shooter enemies have additional logic for projectile firing behavior.
     *
     * @param game The game instance to manage enemy-related resources.
     * @param position The initial position of the shooter enemy.
     * @param enemyId A unique identifier for the shooter enemy.
     * @return The created shooter enemy entity.
     */
    static core::ecs::Entity createShooterEnemy(Game &game, const sf::Vector2f& position, std::uint8_t enemyId);

    /**
     * @brief Creates a game event manager entity to handle global game events.
     *
     * The game event manager is responsible for processing and distributing game-wide events
     * such as player connections, disconnections, game state changes, and other global events.
     * It is assigned an EventComponent and acts as a central hub for event handling.
     *
     * @param game The game instance to manage event-related resources.
     * @return The created game event manager entity.
     * 
     * @see EventComponent For the component that enables event handling capabilities.
     */
    static core::ecs::Entity createGameEventManager(Game &game);
};

#endif // ENTITY_FACTORY_HPP
