#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "../../../core/ecs/Registry/Registry.hpp"

namespace Systems {
    void worldSystem(core::ecs::Registry &registry, const std::array<std::optional<core::ecs::Entity>, 4> &players);
    void playerSystem(core::ecs::Registry &registry, std::array<std::optional<core::ecs::Entity>, 4> &players);
    void enemySystem(core::ecs::Registry &registry, const std::array<std::optional<core::ecs::Entity>, 4> &players);
    void projectileSystem(core::ecs::Registry &registry);
};

#endif //SYSTEMS_HPP
