#include "Game.hpp"

void Game::run()
{
    while (_windowOpen) {
        processEvents();
        update();
        render();
    }
}

void Game::addSystems()
{
    _registry.add_system<Position, Velocity, ClockComponent>(
        [](Position &pos, const Velocity &vel, ClockComponent &)
        {
            pos.x += vel.dx;
            pos.y += vel.dy;
        });

    _registry.add_system<Drawable, Position>([](Drawable &drawable, const Position &pos)
                                             { drawable.shape.setPosition(pos.x, pos.y); });

    _registry.add_system<Drawable>([this](Drawable &drawable) { _window.draw(drawable.shape); });

    _registry.add_system<InputStateComponent>(
        [](InputStateComponent &input)
        {
            // if (input.up) {
            //     printf("up\n");
            // }
            // if (input.down) {
            //     printf("down\n");
            // }
            // if (input.left) {
            //     printf("left\n");
            // }
            // if (input.right) {
            //     printf("right\n");
            // }
            // if (input.fire) {
            //     printf("fire\n");
            // }
        });
}

void Game::init()
{
    _window.create(sf::VideoMode(800, 600), "ECS Game");
    _window.setFramerateLimit(60);

    _registry.register_component<Position>();
    _registry.register_component<Velocity>();
    _registry.register_component<Drawable>();
    _registry.register_component<ClockComponent>();
    _registry.register_component<KeyBinding>();
    _registry.register_component<InputStateComponent>();

    _registry.add_component(_playerEntity, Position{200.0f, 200.0f});
    _registry.add_component(_playerEntity, Velocity{10.0f, 0.0f});
    _registry.add_component(_playerEntity, Drawable{sf::RectangleShape(sf::Vector2f(50.0f, 50.0f))});
    _registry.add_component(_playerEntity, ClockComponent{});
    _registry.add_component(_playerEntity, KeyBinding{});
    _registry.add_component(_playerEntity, InputStateComponent{});


    addSystems();
}

void Game::processEvents()
{
    sf::Event event{};
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _windowOpen = false;
            _window.close();
        }

        // if (event.type == sf::Event::KeyPressed) {
        //     core::ecs::SparseArray<std::shared_ptr<KeyBinding>> keyBinding = _registry.get_components<KeyBinding>();
        //     core::ecs::SparseArray<std::shared_ptr<InputStateComponent>> inputState =
        //         _registry.get_components<InputStateComponent>();

        //     for (auto &binding : keyBinding) {
        //         if (event.key.code == key->up) {
        //             inputState[entity]->up = true;
        //         }
        //         if (event.key.code == key->down) {
        //             inputState[entity]->down = true;
        //         }
        //         if (event.key.code == key->left) {
        //             inputState[entity]->left = true;
        //         }
        //         if (event.key.code == key->right) {
        //             inputState[entity]->right = true;
        //         }
        //         if (event.key.code == key->fire) {
        //             inputState[entity]->fire = true;
        //         }
        //     }
        // }
    }
}


void Game::update()
{
    _registry.run_system<Position, Velocity, ClockComponent>();
    _registry.run_system<Drawable, Position>();
}

void Game::render()
{
    _window.clear();
    _registry.run_system<Drawable>();
    _window.display();
}
