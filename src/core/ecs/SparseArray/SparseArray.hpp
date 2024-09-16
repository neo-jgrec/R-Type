#include <vector>
#include <optional>
#include <functional>

namespace core::ecs {
    template <typename Component>
    class SparseArray {

        public:
            using value_type = std::optional<Component>;
            using reference_type = std::reference_wrapper<Component>;
            using const_reference_type = std::reference_wrapper<const Component>;
            using container_t = std::vector<value_type>;
            using size_type = typename container_t::size_type;

            using iterator = typename container_t::iterator;
            using const_iterator = typename container_t::const_iterator;

        private:
            container_t data;

        public:
            SparseArray() = default;

            SparseArray(SparseArray const &other) = default;
            SparseArray(SparseArray &&other) noexcept = default;
            ~SparseArray() = default;

            SparseArray &operator=(SparseArray const &other) = default;
            SparseArray &operator=(SparseArray &&other) noexcept = default;

            reference_type operator[](size_t index) { return std::ref(data[index]); }
            const_reference_type operator[](size_t index) const { return std::cref(data[index]); }

            iterator begin() { return data.begin(); }
            const_iterator begin() const { return data.begin(); }
            const_iterator cbegin() const { return data.cbegin(); }

            iterator end() { return data.end(); }
            const_iterator end() const { return data.end(); }
            const_iterator cend() const { return data.cend(); }

            size_type size() const { return data.size(); }

            reference_type insert_at(size_t index, Component const &component);
            reference_type insert_at(size_t index, Component &&component);

            template <class... Params>
            reference_type emplace_at(size_t index, Params &&...params);

            void erase(size_type pos);

            size_type get_index(value_type const &value) const;
    };
} // namespace core::ecs
