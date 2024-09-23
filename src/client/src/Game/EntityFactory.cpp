#include "EntityFactory.hpp"

core::ecs::Entity EntityFactory::createPlayer(core::ecs::Registry& registry) {
 
    core::ecs::Entity player = registry.spawn_entity();

    registry.add_component(player, TransformComponent{sf::Vector2f(200.0f, 200.0f), sf::Vector2f(50.0f, 50.0f), sf::Vector2f(1.0f, 1.0f), 0.0f});
    registry.add_component(player, VelocityComponent{10.0f, 10.0f});
    registry.add_component(player, DrawableComponent{sf::RectangleShape(sf::Vector2f(50.0f, 50.0f))});
    registry.add_component(player, InputStateComponent{});
    registry.add_component(player, KeyBinding{});

    registry.add_component(player, HealthComponent{100});
    registry.add_component(player, ScoreComponent{0});

    return player;
}
