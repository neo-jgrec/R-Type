#include <unordered_map>
#include <vector>
#include <any>
#include <typeindex>
#include <functional>

#include "../Entity/Entity.hpp"
#include "../SparseArray/SparseArray.hpp"

namespace core::ecs {
    class Registry {
        private:
            std::unordered_map<std::type_index, std::any> _components_array;
            std::unordered_map<std::type_index, std::function<void(Registry&, Entity const&)>> _erase_functions;
            std::vector<Entity> _dead_entities;
            size_t _next_entity_id = 0;

        public:
            template <class Component>
            SparseArray<Component>& register_component();

            template <class Component>
            SparseArray<Component>& get_components() { return std::any_cast<SparseArray<Component>&>(_components_array.at(std::type_index(typeid(Component)))); }

            template <class Component>
            SparseArray<Component> const& get_components() const { return std::any_cast<SparseArray<Component> const&>(_components_array.at(std::type_index(typeid(Component)))); }
            static Entity entity_from_index(std::size_t idx) { return Entity(idx); }

            Entity spawn_entity();
            void kill_entity(Entity const& entity);

            template <typename Component>
            typename SparseArray<Component>::reference_type add_component(Entity const& to, Component&& c) { return get_components<Component>().insert_at(static_cast<size_t>(to), std::forward<Component>(c)); }

            template <typename Component, typename... Params>
            typename SparseArray<Component>::reference_type emplace_component(Entity const& to, Params&&... p) { return get_components<Component>().emplace_at(static_cast<size_t>(to), std::forward<Params>(p)...); }

            template <typename Component>
            void remove_component(Entity const& from) { get_components<Component>().erase(static_cast<size_t>(from)); }
    };
} // namespace core::ecs
