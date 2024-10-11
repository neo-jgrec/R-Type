#pragma once

#include <algorithm>
#include <optional>
#include <vector>

namespace core::ecs {

/**
 * @class SparseArray
 * @brief A container that efficiently stores optional components, allowing for sparse storage of data.
 * 
 * The `SparseArray` class is designed to store components in a sparse manner, where each index represents an entity ID,
 * and the component at that index may or may not exist. It provides operations to insert, erase, and access components
 * by index while maintaining efficiency.
 * 
 * @tparam Component The type of the component to be stored in the sparse array.
 */
template <typename Component>
class SparseArray {
public:
    using value_type = std::optional<Component>;               ///< The type of the values stored in the array (optional components).
    using reference_type = value_type &;                       ///< A reference to a component in the array.
    using const_reference_type = value_type const &;           ///< A const reference to a component in the array.
    using container_t = std::vector<value_type>;               ///< The underlying container type (a vector of optional components).
    using size_type = typename container_t::size_type;         ///< The size type used by the container.
    using iterator = typename container_t::iterator;           ///< Iterator type for the container.
    using const_iterator = typename container_t::const_iterator; ///< Const iterator type for the container.

    /** @brief Default constructor. */
    SparseArray() = default;

    /** @brief Default copy constructor. */
    SparseArray(SparseArray const &) = default;

    /** @brief Default move constructor. */
    SparseArray(SparseArray &&) noexcept = default;

    /** @brief Default destructor. */
    ~SparseArray() = default;

    /** @brief Default copy assignment operator. */
    SparseArray &operator=(SparseArray const &) = default;

    /** @brief Default move assignment operator. */
    SparseArray &operator=(SparseArray &&) noexcept = default;

    /**
     * @brief Accesses or inserts a component at the specified index.
     * 
     * If the index is out of bounds, the container is resized to accommodate the index.
     * 
     * @param idx The index at which the component is accessed or inserted.
     * @return Reference to the component at the specified index.
     */
    reference_type operator[](size_t idx) {
        if (idx >= _data.size()) {
            _data.resize(idx + 1);
        }
        return _data[idx];
    }

    /**
     * @brief Accesses a component at the specified index (const version).
     * 
     * If the index is out of bounds, a default empty value is returned.
     * 
     * @param idx The index at which the component is accessed.
     * @return Const reference to the component at the specified index.
     */
    const_reference_type operator[](size_t idx) const {
        if (idx >= _data.size()) {
            static const value_type empty;
            return empty;
        }
        return _data[idx];
    }

    /** @brief Returns an iterator to the beginning of the container. */
    iterator begin() { return _data.begin(); }

    /** @brief Returns a const iterator to the beginning of the container. */
    const_iterator begin() const { return _data.begin(); }

    /** @brief Returns a const iterator to the beginning of the container. */
    const_iterator cbegin() const { return _data.cbegin(); }

    /** @brief Returns an iterator to the end of the container. */
    iterator end() { return _data.end(); }

    /** @brief Returns a const iterator to the end of the container. */
    const_iterator end() const { return _data.end(); }

    /** @brief Returns a const iterator to the end of the container. */
    const_iterator cend() const { return _data.cend(); }

    /**
     * @brief Returns the number of elements in the array, including empty slots.
     * 
     * @return The size of the underlying container.
     */
    size_type size() const { return _data.size(); }

    /**
     * @brief Inserts a component at the specified index.
     * 
     * If the index is out of bounds, the container is resized to accommodate the index.
     * 
     * @param pos The index at which to insert the component.
     * @param comp The component to insert.
     * @return Reference to the inserted component.
     */
    reference_type insert_at(size_type pos, Component const &comp) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = comp;
        return _data[pos];
    }

    /**
     * @brief Inserts a component at the specified index (move version).
     * 
     * Moves the component into the container, resizing if necessary.
     * 
     * @param pos The index at which to insert the component.
     * @param comp The component to insert (rvalue).
     * @return Reference to the inserted component.
     */
    reference_type insert_at(size_type pos, Component &&comp) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = std::move(comp);
        return _data[pos];
    }

    /**
     * @brief Constructs a component in place at the specified index.
     * 
     * If the index is out of bounds, the container is resized to accommodate the index.
     * 
     * @tparam Params Parameter pack for the component constructor.
     * @param pos The index at which to construct the component.
     * @param params The parameters to forward to the component constructor.
     * @return Reference to the newly emplaced component.
     */
    template <class... Params>
    reference_type emplace_at(size_type pos, Params &&...params) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos].emplace(std::forward<Params>(params)...);
        return _data[pos];
    }

    /**
     * @brief Removes a component at the specified index.
     * 
     * The component is removed from the array, and its slot is reset.
     * 
     * @param pos The index at which to erase the component.
     */
    void erase(size_type pos) {
        if (pos < _data.size()) {
            _data[pos].reset();
        }
    }

    /**
     * @brief Finds the index of a component in the array.
     * 
     * @param comp The component to find.
     * @return The index of the component, or -1 if not found.
     */
    size_type get_index(value_type const &comp) const {
        auto it = std::find(_data.begin(), _data.end(), comp);
        if (it != _data.end()) {
            return std::distance(_data.begin(), it);
        }
        return static_cast<size_type>(-1);
    }

private:
    container_t _data; ///< The underlying container holding the optional components.
};

} // namespace core::ecs
