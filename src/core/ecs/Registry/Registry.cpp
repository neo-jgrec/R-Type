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
