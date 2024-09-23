#include "Game/Game.hpp"
#include "../../core/ecs/Registry/Registry.hpp"

int main()
{
    core::ecs::Registry registry;
    Game game(registry);
    game.run();
    return 0;
}

