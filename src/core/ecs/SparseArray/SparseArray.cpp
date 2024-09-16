#include "SparseArray.hpp"
#include <algorithm>

using namespace core::ecs;

template <typename Component>
typename SparseArray<Component>::reference_type SparseArray<Component>::insert_at(size_t index, Component const &component)
{
    if (index >= data.size()) {
        data.resize(index + 1);
    }

    data[index] = component;
    return std::ref(data[index]);
}

template <typename Component>
typename SparseArray<Component>::reference_type SparseArray<Component>::insert_at(size_t index, Component &&component)
{
    if (index >= data.size()) {
        data.resize(index + 1);
    }

    data[index] = std::move(component);
    return std::ref(data[index]);
}

template <typename Component>
template <class... Params>
typename SparseArray<Component>::reference_type SparseArray<Component>::emplace_at(size_t index, Params &&...params)
{
    if (index >= data.size()) {
        data.resize(index + 1);
    }

    data[index] = Component(std::forward<Params>(params)...);
    return std::ref(data[index]);
}

template <typename Component>
void SparseArray<Component>::erase(typename SparseArray<Component>::size_type pos)
{
    if (pos < data.size()) {
        data[pos].reset();
    }
}

template <typename Component>
typename SparseArray<Component>::size_type SparseArray<Component>::get_index(typename SparseArray<Component>::value_type const &value) const
{
    auto it = std::find(data.begin(), data.end(), value);
    return it != data.end() ? std::distance(data.begin(), it) : data.size();
}
