#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include "../../../core/ecs/Registry/Registry.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"

class EntityFactory {
public:
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position, int color);
    static core::ecs::Entity createPlayerProjectile(core::ecs::Registry& registry, core::ge::TransformComponent& playerTransform);
    static core::ecs::Entity createPlayerMissile(core::ecs::Registry& registry, core::ge::TransformComponent& playerTransform);
    static core::ecs::Entity createEnemy(core::ecs::Registry& registry, const sf::Vector2f& position);

};

#endif // ENTITY_FACTORY_HPP
