#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>
#include <stdexcept>

#include "../Entity/Entity.hpp"
#include "../SparseArray/SparseArray.hpp"

namespace core::ecs {

/**
 * @class IComponentArray
 * @brief Interface for managing component arrays.
 * 
 * The `IComponentArray` interface provides a base class for managing collections of components
 * in the entity-component system (ECS). This allows components to be stored in type-erased containers.
 */
class IComponentArray {
public:
    virtual ~IComponentArray() = default;

    /**
     * @brief Erase a component by its entity ID.
     * 
     * This method removes the component associated with the given entity ID.
     * 
     * @param id The entity ID whose component should be erased.
     */
    virtual void erase(size_t id) = 0;
};

/**
 * @class ComponentArray
 * @brief Manages a sparse array of components for a specific type.
 * 
 * The `ComponentArray` class manages components of a specific type, allowing components to be
 * added, removed, and retrieved by entity ID.
 * 
 * @tparam Component The type of component stored in this array.
 */
template <typename Component>
class ComponentArray : public IComponentArray {
public:
    SparseArray<std::shared_ptr<Component>> data; ///< Sparse array of components.

    /**
     * @brief Erase the component associated with a given entity ID.
     * 
     * @param id The entity ID whose component should be erased.
     */
    void erase(size_t id) override {
        data.erase(id);
    }
};

/**
 * @class Registry
 * @brief Manages entities and their associated components in the ECS.
 * 
 * The `Registry` class serves as the core of the entity-component system (ECS), allowing for the registration of components,
 * the creation of entities, and the execution of systems. It stores and manages components for each entity and facilitates
 * the addition and removal of components dynamically.
 */
class Registry {
public:
    /**
     * @brief Registers a new component type in the ECS.
     * 
     * Registers a component type in the ECS and returns a reference to the sparse array
     * containing all instances of the component.
     * 
     * @tparam Component The type of the component to register.
     * @return Reference to the sparse array of components of the registered type.
     */
    template <class Component>
    SparseArray<std::shared_ptr<Component>> &register_component() {
        std::type_index index = typeid(Component);
        auto it = _components_arrays.find(index);
        if (it == _components_arrays.end()) {
            _components_arrays[index] = std::make_shared<ComponentArray<Component>>();
        }
        return static_cast<ComponentArray<Component>&>(*_components_arrays[index]).data;
    }

    /**
     * @brief Retrieves the array of all components of a specified type.
     * 
     * @tparam Component The type of the component to retrieve.
     * @return Reference to the sparse array containing all instances of the component.
     */
    template <class Component>
    SparseArray<std::shared_ptr<Component>> &get_components() {
        return static_cast<ComponentArray<Component>&>(*_components_arrays[typeid(Component)]).data;
    }

    /**
     * @brief Retrieves the array of all components of a specified type (const version).
     * 
     * @tparam Component The type of the component to retrieve.
     * @return Const reference to the sparse array containing all instances of the component.
     */
    template <class Component>
    SparseArray<std::shared_ptr<Component>> const &get_components() const {
        return static_cast<const ComponentArray<Component>&>(*_components_arrays.at(typeid(Component))).data;
    }

    /**
     * @brief Retrieves a specific component instance associated with an entity.
     * 
     * If the entity does not have the specified component, a runtime error is thrown.
     * 
     * @tparam Component The type of the component to retrieve.
     * @param e The entity whose component is being retrieved.
     * @return Shared pointer to the component instance.
     */
    template <class Component>
    std::shared_ptr<Component> get_component(Entity const &e) {
        auto &comp_array = get_components<Component>();
        auto value = comp_array[static_cast<size_t>(e)];
        if (!value.has_value())
            throw std::runtime_error("Entity does not have component");
        return value.value();
    }

    /**
     * @brief Spawns a new entity and returns it.
     * 
     * This method creates a new entity with a unique ID.
     * 
     * @return The newly spawned entity.
     */
    Entity spawn_entity() {
        size_t id = _next_entity_id++;
        return Entity{id};
    }

    /**
     * @brief Removes an entity and all its associated components.
     * 
     * This method deletes an entity and erases all components associated with it.
     * 
     * @param e The entity to be killed.
     */
    void kill_entity(Entity const &e) {
        for (auto &[_, component_array] : _components_arrays) {
            component_array->erase(static_cast<size_t>(e));
        }
    }

    /**
     * @brief Adds a component to an entity.
     * 
     * @tparam Component The type of the component to add.
     * @param to The entity to which the component will be added.
     * @param c The component to add.
     * @return Reference to the inserted component.
     */
    template <typename Component>
    typename SparseArray<std::shared_ptr<Component>>::reference_type add_component(Entity const &to, Component &&c) {
        auto &comp_array = get_components<Component>();
        return comp_array.insert_at(static_cast<size_t>(to),
                                    std::make_shared<Component>(std::forward<Component>(c)));
    }

    /**
     * @brief Adds a component to an entity by constructing it in place.
     * 
     * @tparam Component The type of the component to add.
     * @tparam Params The types of the parameters for the component's constructor.
     * @param to The entity to which the component will be added.
     * @param params The parameters to pass to the component's constructor.
     * @return Reference to the newly emplaced component.
     */
    template <typename Component, typename... Params>
    typename SparseArray<std::shared_ptr<Component>>::reference_type emplace_component(Entity const &to, Params &&...params) {
        auto &comp_array = get_components<Component>();
        return comp_array.emplace_at(static_cast<size_t>(to),
                                     std::make_shared<Component>(std::forward<Params>(params)...));
    }

    /**
     * @brief Removes a component from an entity.
     * 
     * @tparam Component The type of the component to remove.
     * @param from The entity from which the component will be removed.
     */
    template <typename Component>
    void remove_component(Entity const &from) {
        auto &comp_array = get_components<Component>();
        comp_array.erase(static_cast<size_t>(from));
    }

    /**
     * @brief Adds a system to the ECS.
     * 
     * Systems are functions that operate on entities with specific components. This method adds a system
     * that will be executed on entities with the specified components.
     * 
     * @tparam Components The component types that the system will operate on.
     * @tparam Function The type of the system function.
     * @param f The system function to add.
     */
    template <class... Components, typename Function>
    void add_system(Function &&f) {
        _systems.emplace_back([this, f = std::forward<Function>(f)](Registry &r) {
            call_system<Components...>(f, r, std::index_sequence_for<Components...>{});
        }, std::vector<std::type_index>{typeid(Components)...});
    }

    /**
     * @brief Runs all systems that have been added to the ECS.
     * 
     * This method executes all registered systems.
     */
    void run_systems() {
        for (auto &[system, components] : _systems) {
            system(*this);
        }
    }

    /**
     * @brief Runs a specific system based on its component types.
     * 
     * @tparam Components The component types of the system to run.
     */
    template <class... Components>
    void run_system() {
        std::vector<std::type_index> component_types = {typeid(Components)...};

        for (auto &[system, components] : _systems) {
            if (components == component_types) {
                system(*this);
            }
        }
    }

    /**
     * @brief Retrieves entities that have all specified components.
     * 
     * @tparam Components The types of components to check for.
     * @return A vector of entities that have all specified components.
     */
    template <typename... Components>
    std::vector<Entity> get_entities() {
        std::vector<Entity> entities;
        auto &first_component = get_components<std::tuple_element_t<0, std::tuple<Components...>>>();
        for (size_t i = 0; i < first_component.size(); ++i) {
            if (are_components_present<Components...>(i)) {
                entities.emplace_back(i);
            }
        }
        return entities;
    }

    /**
     * @brief Checks if an entity has a specific component.
     * 
     * @tparam Component The type of the component to check for.
     * @param e The entity to check.
     * @return True if the entity has the component, false otherwise.
     */
    template <typename Component>
    bool has_component(Entity const &e) const {
        auto &comp_array = get_components<Component>();
        return comp_array.size() > static_cast<size_t>(e) && comp_array[static_cast<size_t>(e)].has_value();
    }

private:
    /**
     * @brief Helper method to call a system function with the appropriate components.
     * 
     * This method iterates over the entities and applies the system function to entities
     * that have all the required components.
     * 
     * @tparam Components The types of components the system operates on.
     * @tparam Function The type of the system function.
     * @tparam Is A parameter pack of indices used to iterate over components.
     * @param f The system function to call.
     * @param r The registry instance.
     */
    template <typename... Components, typename Function, std::size_t... Is>
    void call_system(Function &&f, Registry &r, std::index_sequence<Is...>) {
        auto component_arrays = std::make_tuple(r.get_components<Components>()...);
        size_t array_size = std::get<0>(component_arrays).size();

        for (size_t i = 0; i < array_size; ++i) {
            if (are_components_present<Components...>(i)) {
                std::apply([i, &f](auto &...arrays) { f(Entity{i}, *(arrays[i].value())...); }, component_arrays);
            }
        }
    }

    /**
     * @brief Checks if an entity has all specified components.
     * 
     * @tparam Components The types of components to check for.
     * @param id The ID of the entity to check.
     * @return True if the entity has all specified components, false otherwise.
     */
    template <typename... Components>
    bool are_components_present(size_t id) const {
        return (... && get_components<Components>()[id].has_value());
    }

    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> _components_arrays; ///< Maps component types to their arrays.
    size_t _next_entity_id = 0; ///< Tracks the next available entity ID.
    std::vector<std::pair<std::function<void(Registry &)>, std::vector<std::type_index>>> _systems; ///< List of systems in the ECS.
};

} // namespace core::ecs

#endif /* !REGISTRY_HPP */
