#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

/**
 * @struct InputStateComponent
 * @brief Represents the input state for controlling player movement and actions.
 * 
 * This component stores boolean flags indicating whether certain keys are pressed or not (e.g., up, down, left, right, fire).
 * It is used to manage player input in the game.
 */
struct InputStateComponent {
    bool up = false;   ///< Indicates if the 'up' key is pressed.
    bool down = false; ///< Indicates if the 'down' key is pressed.
    bool left = false; ///< Indicates if the 'left' key is pressed.
    bool right = false; ///< Indicates if the 'right' key is pressed.
    bool fire = false; ///< Indicates if the 'fire' key is pressed.
    bool fireReleased = false; ///< Indicates if the 'fire' key was released.
};

/**
 * @struct HealthComponent
 * @brief Stores the health of an entity.
 * 
 * The health component tracks the health of an entity, reducing when damage is taken. It is primarily used for player and enemy entities.
 */
struct HealthComponent {
    int health; ///< The current health of the entity.
};

/**
 * @struct DamageComponent
 * @brief Defines the amount of damage an entity can inflict.
 * 
 * This component is typically used for projectiles or entities that can cause damage, specifying how much damage they deal.
 */
struct DamageComponent {
    int damage; ///< The amount of damage inflicted on collision or interaction.
};

/**
 * @struct ScoreComponent
 * @brief Tracks the score of a player.
 * 
 * This component holds the player's current score, which is updated based on game events such as defeating enemies.
 */
struct ScoreComponent {
    int score; ///< The current score of the player.
};

/**
 * @struct ShootCounterComponent
 * @brief Manages the shooting cooldown or shot count for a player.
 * 
 * This component tracks how many shots have been fired or time between shots, allowing for control over shooting frequency.
 */
struct ShootCounterComponent {
    float shotCount; ///< The current shot count or cooldown timer for shooting.
    float chargeTime; ///< The time taken to charge the shot.
    float notChargingTime; ///< The time taken to not charge the shot.
    int nextShotType; ///< The type of the next shot to be fired.
};

/**
 * @struct PlayerColorComponent
 * @brief Stores the color identifier for a player.
 * 
 * This component is used to manage the color of a player's sprite, which can be customized or assigned dynamically.
 */
struct PlayerColorComponent {
    int color; ///< The color identifier for the player's sprite.
};

// Identifiers

/**
 * @struct Player
 * @brief Tag component used to identify an entity as a player.
 * 
 * This tag is applied to player entities to distinguish them in systems or logic that targets players.
 */
struct Player {
    std::uint8_t id = 0;
    bool self = false;
};

/**
 * @struct PlayerAnim
 * @brief Tag component used to identify an entity as a player animation.
 * 
 * This tag is applied to player animation entities to distinguish them in systems or logic that targets player animations.
 */
struct PlayerAnim {
    std::uint8_t id = 0;
};

/**
 * @struct Enemy
 * @brief Tag component used to identify an entity as an enemy.
 * 
 * This tag is applied to enemy entities to distinguish them in systems or logic that targets enemies.
 */
struct Enemy {
    std::uint8_t id = 0;
};

/**
 * @struct Projectile
 * @brief Tag component used to identify an entity as a projectile.
 * 
 * This tag is applied to entities that represent projectiles, used to distinguish them from other entities.
 */
struct Projectile {};

/**
 * @struct Missile
 * @brief Tag component used to identify an entity as a missile.
 * 
 * This tag is applied to missile entities, which may differ from regular projectiles in terms of behavior and damage.
 */
struct Missile {};

/**
 * @struct IndicatorComponent
 * @brief Tag component used to identify an entity as an indicator component.
 * 
 * This tag is applied to enemy entites to display visual clues about them.
 */
struct IndicatorComponent {
    sf::Vector2f pos;
    bool isEnemyOffscreen = true;
    sf::CircleShape shape;
};
