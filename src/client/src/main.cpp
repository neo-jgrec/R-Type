#include <SFML/Graphics.hpp>
#include <iostream>
#include "../../core/ecs/Registry/Registry.hpp"

struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

struct Drawable {
    sf::RectangleShape shape;
};

struct ClockComponent {
    sf::Clock clock;
    sf::Time elapsedTime;
};

void move_entity(Position &position, const Velocity &velocity)
{
    position.x += velocity.dx;
    position.y += velocity.dy;
}

void update_drawable(Drawable &drawable, const Position &position)
{
    drawable.shape.setPosition(position.x, position.y);
    std::cout << "Updated drawable to: " << position.x << ", " << position.y << std::endl;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "ECS SFML Example");

    core::ecs::Registry registry;

    registry.register_component<Position>();
    registry.register_component<Velocity>();
    registry.register_component<Drawable>();
    registry.register_component<ClockComponent>();

    auto entity = registry.spawn_entity();

    registry.add_component(entity, Position{200.0f, 200.0f});
    registry.add_component(entity, Velocity{10.0f, 0.0f});
    registry.add_component(entity, Drawable{sf::RectangleShape(sf::Vector2f(50.0f, 50.0f))});
    registry.add_component(entity, ClockComponent{});

    registry.add_system<Position, Velocity, ClockComponent>(
        [](Position &pos, const Velocity &vel, ClockComponent &clockComp) {
            clockComp.elapsedTime = clockComp.clock.getElapsedTime();
            if (clockComp.elapsedTime.asSeconds() >= 1.0f) {
                move_entity(pos, vel);
                std::cout << "Moved entity to: " << pos.x << ", " << pos.y << std::endl;

                clockComp.clock.restart();
            }
        });

    registry.add_system<Drawable, Position>(
        [](Drawable &drawable, const Position &pos) {
            update_drawable(drawable, pos);
        });

    registry.add_system<Drawable>([&window](Drawable &drawable) {
        window.draw(drawable.shape);
    });

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        registry.run_system<Position, Velocity, ClockComponent>();

        registry.run_system<Drawable, Position>();

        registry.run_system<Drawable>();

        window.display();
    }

    return 0;
}
