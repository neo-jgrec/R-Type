#include <algorithm>
#include <vector>
#include <optional>

#pragma once
namespace core::ecs {
    template <typename Component>
    class SparseArray {
    public:
        using value_type = std::optional<Component>;
        using reference_type = value_type&;
        using const_reference_type = value_type const&;
        using container_t = std::vector<value_type>;
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

        SparseArray() = default;
        SparseArray(SparseArray const&) = default;
        SparseArray(SparseArray&&) noexcept = default;
        ~SparseArray() = default;

        SparseArray& operator=(SparseArray const&) = default;
        SparseArray& operator=(SparseArray&&) noexcept = default;

        reference_type operator[](size_t idx) {
            if (idx >= _data.size()) {
                _data.resize(idx + 1);
            }
            return _data[idx];
        }

        const_reference_type operator[](size_t idx) const {
            if (idx >= _data.size()) {
                static const value_type empty;
                return empty;
            }
            return _data[idx];
        }

        iterator begin() { return _data.begin(); }
        const_iterator begin() const { return _data.begin(); }
        const_iterator cbegin() const { return _data.cbegin(); }
        iterator end() { return _data.end(); }
        const_iterator end() const { return _data.end(); }
        const_iterator cend() const { return _data.cend(); }

        size_type size() const { return _data.size(); }

        reference_type insert_at(size_type pos, Component const& comp) {
            if (pos >= _data.size()) {
                _data.resize(pos + 1);
            }
            _data[pos] = comp;
            return _data[pos];
        }

        reference_type insert_at(size_type pos, Component&& comp) {
            if (pos >= _data.size()) {
                _data.resize(pos + 1);
            }
            _data[pos] = std::move(comp);
            return _data[pos];
        }

        template <class... Params>
        reference_type emplace_at(size_type pos, Params&&... params) {
            if (pos >= _data.size()) {
                _data.resize(pos + 1);
            }
            _data[pos].emplace(std::forward<Params>(params)...);
            return _data[pos];
        }

        void erase(size_type pos) {
            if (pos < _data.size()) {
                _data[pos].reset();
            }
        }

        size_type get_index(value_type const& comp) const {
            auto it = std::find(_data.begin(), _data.end(), comp);
            if (it != _data.end()) {
                return std::distance(_data.begin(), it);
            }
            return static_cast<size_type>(-1);
        }

    private:
        container_t _data;
    };
} // namespace core::ecs
