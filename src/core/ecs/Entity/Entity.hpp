#include <cstddef>

#pragma once
namespace core::ecs {
    class Entity {
        private:
            size_t id;

        public:
            explicit Entity(size_t id) : id(id) {}
            operator size_t() const { return id; }
    };
}
