#include "Registry.hpp"

using namespace core::ecs;

template <class Component>
SparseArray<Component>& Registry::register_component()
{
    auto typeIndex = std::type_index(typeid(Component));
    if (_components_array.find(typeIndex) == _components_array.end()) {
        _components_array[typeIndex] = SparseArray<Component>();
        _erase_functions[typeIndex] = [](Registry& registry, Entity const& entity) {
            registry.get_components<Component>().erase(static_cast<size_t>(entity));
        };
    }
    return std::any_cast<SparseArray<Component>&>(_components_array[typeIndex]);
}

Entity Registry::spawn_entity()
{
    if (!_dead_entities.empty()) {
        Entity entity = _dead_entities.back();
        _dead_entities.pop_back();
        return entity;
    }
    return Entity(_next_entity_id++);
}

void Registry::kill_entity(Entity const& entity)
{
    for (auto& [type, func] : _erase_functions) {
        func(*this, entity);
    }
    _dead_entities.push_back(entity);
}

template <class ...Components, typename Function>
void Registry::add_system(Function&& f)
{
    auto const& components = std::vector<std::type_index>{std::type_index(typeid(Components))...};
    auto const& erase_function = [f](Registry& r, Entity const& e) {
        f(r, e, r.get_components<Components>()[static_cast<size_t>(e)]...);
    };
    for (auto const& component : components) {
        _erase_functions[component] = erase_function;
    }
}

void Registry::run_systems()
{
    for (auto const& entity : _dead_entities) {
        for (auto& [type, func] : _erase_functions) {
            func(*this, entity);
        }
    }
    _dead_entities.clear();
}
