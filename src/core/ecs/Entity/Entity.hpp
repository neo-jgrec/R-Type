#pragma once

#include <cstddef>

namespace core::ecs {

/**
 * @class Entity
 * @brief Represents an entity in the entity-component system (ECS).
 * 
 * The `Entity` class serves as a lightweight wrapper around a unique ID, representing an entity
 * in the ECS. Each entity is identified by its unique `id` and can be used to associate components.
 */
class Entity {
private:
    size_t id; ///< The unique identifier for this entity.

public:
    /**
     * @brief Default constructor for the Entity class.
     * 
     * Creates an entity with an uninitialized ID.
     */
    Entity() = default;

    /**
     * @brief Default destructor for the Entity class.
     */
    ~Entity() = default;

    /**
     * @brief Constructs an Entity with a specific ID.
     * 
     * @param id The unique identifier for the entity.
     */
    explicit Entity(size_t id) : id(id) {}

    /**
     * @brief Implicit conversion operator to size_t.
     * 
     * This allows the entity to be used as a `size_t` in contexts where its ID is needed.
     * 
     * @return The unique ID of the entity as a `size_t`.
     */
    operator size_t() const { return id; }
};

} // namespace core::ecs
