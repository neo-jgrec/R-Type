#include <unordered_map>
#include <vector>
#include <any>
#include <typeindex>
#include <optional>
#include <functional>
#include <iostream>

#include "../Entity/Entity.hpp"
#include "../SparseArray/SparseArray.hpp"

namespace core {
    namespace ecs {
        class Registry {
            private:
                std::unordered_map<std::type_index, std::any> _components_array;
                std::unordered_map<std::type_index, std::function<void(Registry&, Entity const&)>> _erase_functions;
                std::vector<Entity> _dead_entities;
                size_t _next_entity_id = 0;

            public:
                template <class Component>
                SparseArray<Component>& register_component() {
                    auto typeIndex = std::type_index(typeid(Component));
                    if (_components_array.find(typeIndex) == _components_array.end()) {
                        _components_array[typeIndex] = SparseArray<Component>();
                        _erase_functions[typeIndex] = [](Registry& registry, Entity const& entity) {
                            registry.get_components<Component>().erase(static_cast<size_t>(entity));
                        };
                    }
                    return std::any_cast<SparseArray<Component>&>(_components_array[typeIndex]);
                }

                template <class Component>
                SparseArray<Component>& get_components() {
                    return std::any_cast<SparseArray<Component>&>(_components_array.at(std::type_index(typeid(Component))));
                }

                template <class Component>
                SparseArray<Component> const& get_components() const {
                    return std::any_cast<SparseArray<Component> const&>(_components_array.at(std::type_index(typeid(Component))));
                }

                Entity spawn_entity() {
                    if (!_dead_entities.empty()) {
                        Entity entity = _dead_entities.back();
                        _dead_entities.pop_back();
                        return entity;
                    }
                    return Entity(_next_entity_id++);
                }

                static Entity entity_from_index(std::size_t idx) {
                    return Entity(idx);
                }

                void kill_entity(Entity const& entity) {
                    for (auto& [type, func] : _erase_functions) {
                        func(*this, entity);
                    }
                    _dead_entities.push_back(entity);
                }

                template <typename Component>
                typename SparseArray<Component>::reference_type add_component(Entity const& to, Component&& c) {
                    return get_components<Component>().insert_at(static_cast<size_t>(to), std::forward<Component>(c));
                }

                template <typename Component, typename... Params>
                typename SparseArray<Component>::reference_type emplace_component(Entity const& to, Params&&... p) {
                    return get_components<Component>().emplace_at(static_cast<size_t>(to), std::forward<Params>(p)...);
                }

                template <typename Component>
                void remove_component(Entity const& from) {
                    get_components<Component>().erase(static_cast<size_t>(from));
                }
        };
    } // namespace ecs
} // namespace core
