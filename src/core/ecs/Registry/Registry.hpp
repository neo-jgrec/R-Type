#pragma once

#include <any>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../Entity/Entity.hpp"
#include "../SparseArray/SparseArray.hpp"

namespace core::ecs
{
    class Registry {
    public:
        template <class Component>
        SparseArray<std::shared_ptr<Component>> &register_component()
        {
            std::type_index index = typeid(Component);
            auto it = _components_arrays.find(index);
            if (it == _components_arrays.end()) {
                _components_arrays[index] = SparseArray<std::shared_ptr<Component>>();
            }
            return std::any_cast<SparseArray<std::shared_ptr<Component>> &>(_components_arrays[index]);
        }

        template <class Component>
        SparseArray<std::shared_ptr<Component>> &get_components()
        {
            return std::any_cast<SparseArray<std::shared_ptr<Component>> &>(_components_arrays[typeid(Component)]);
        }

        template <class Component>
        SparseArray<std::shared_ptr<Component>> const &get_components() const
        {
            return std::any_cast<SparseArray<std::shared_ptr<Component>> const &>(
                _components_arrays.at(typeid(Component)));
        }

        Entity spawn_entity()
        {
            size_t id = _next_entity_id++;
            return Entity{id};
        }

        void kill_entity(Entity const &e)
        {
            if (std::find(_entities_to_kill.begin(), _entities_to_kill.end(), e) == _entities_to_kill.end()) {
                _entities_to_kill.push_back(e);
            }
        }

        template <typename Component>
        typename SparseArray<std::shared_ptr<Component>>::reference_type add_component(Entity const &to, Component &&c)
        {
            auto &comp_array = get_components<Component>();
            return comp_array.insert_at(static_cast<size_t>(to),
                                        std::make_shared<Component>(std::forward<Component>(c)));
        }

        template <typename Component, typename... Params>
        typename SparseArray<std::shared_ptr<Component>>::reference_type emplace_component(Entity const &to,
                                                                                           Params &&...params)
        {
            auto &comp_array = get_components<Component>();
            return comp_array.emplace_at(static_cast<size_t>(to),
                                         std::make_shared<Component>(std::forward<Params>(params)...));
        }

        template <typename Component>
        void remove_component(Entity const &from)
        {
            auto &comp_array = get_components<Component>();
            comp_array.erase(static_cast<size_t>(from));
        }

        template <class... Components, typename Function>
        void add_system(Function &&f)
        {
            _systems.emplace_back([this, f = std::forward<Function>(f)](Registry &r)
                                  { call_system<Components...>(f, r, std::index_sequence_for<Components...>{}); },
                                  std::vector<std::type_index>{typeid(Components)...});
        }

        void run_systems()
        {
            for (auto &[system, components] : _systems) {
                system(*this);
            }
            cleanup_dead_entities();
        }

        template <class... Components>
        void run_system()
        {
            std::vector<std::type_index> component_types = {typeid(Components)...};

            for (auto &[system, components] : _systems) {
                if (components == component_types) {
                    system(*this);
                }
            }
        }

        template <typename... Components>
        std::vector<Entity> get_entities()
        {
            std::vector<Entity> entities;
            auto &first_component = get_components<std::tuple_element_t<0, std::tuple<Components...>>>();
            for (size_t i = 0; i < first_component.size(); ++i) {
                if (are_components_present<Components...>(i)) {
                    entities.emplace_back(i);
                }
            }
            return entities;
        }

    private:
        template <typename... Components, typename Function, std::size_t... Is>
        void call_system(Function &&f, Registry &r, std::index_sequence<Is...>)
        {
            auto component_arrays = std::make_tuple(r.get_components<Components>()...);

            size_t array_size = std::get<0>(component_arrays).size();

            for (size_t i = 0; i < array_size; ++i) {
                if (are_components_present<Components...>(i)) {
                    std::apply([i, &f](auto &...arrays) { f(*(arrays[i].value())...); }, component_arrays);
                }
            }
        }


        template <typename... Components>
        bool are_components_present(size_t id) const
        {
            return (... && get_components<Components>()[id].has_value());
        }

        void cleanup_dead_entities()
        {
            for (const auto &entity : _entities_to_kill) {
                for (auto &[type, array] : _components_arrays) {
                    auto &component_array = std::any_cast<SparseArray<std::shared_ptr<void>> &>(array);
                    component_array.erase(static_cast<size_t>(entity));
                }
            }
            _entities_to_kill.clear();
        }

        std::unordered_map<std::type_index, std::any> _components_arrays;
        size_t _next_entity_id = 0;
        std::vector<std::pair<std::function<void(Registry &)>, std::vector<std::type_index>>> _systems;
        std::vector<Entity> _entities_to_kill;
    };
} // namespace core::ecs
