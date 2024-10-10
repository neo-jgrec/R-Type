#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include "../../../core/ecs/Registry/Registry.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "../../../core/ecs/GameEngine/GameEngineComponents.hpp"

class EntityFactory {
public:
    static core::ecs::Entity createPlayer(core::ecs::Registry& registry, const sf::Vector2f& position, int color, sf::Vector2f gameScale);
    static core::ecs::Entity createPlayerProjectile(core::ecs::Registry& registry, core::ge::TransformComponent& playerTransform, sf::Vector2f gameScale);
    static core::ecs::Entity createPlayerMissile(core::ecs::Registry& registry, core::ge::TransformComponent& playerTransform, sf::Vector2f gameScale);
    static core::ecs::Entity createEnemy(core::ecs::Registry& registry, const sf::Vector2f& position, sf::Vector2f gameScale);
    static core::ecs::Entity createButton(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void()>& onClick, int scene);
    static core::ecs::Entity createTextInput(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& placeholder, const std::function<void()>& onClick, int scene);
    static core::ecs::Entity createSlider(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void(float)>& onChange, int scene, float currentValue);
};

#endif // ENTITY_FACTORY_HPP
