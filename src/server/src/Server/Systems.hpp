#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "../../../core/ecs/Registry/Registry.hpp"

namespace Systems {
    void worldSystem(core::ecs::Registry &registry);
    void playerSystem(core::ecs::Registry &registry, std::array<std::optional<core::ecs::Entity>, 4> &players);
};

#endif //SYSTEMS_HPP
