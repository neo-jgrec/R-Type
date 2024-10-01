#include "Game/Game.hpp"
#include "../../core/ecs/Registry/Registry.hpp"
#include "../../core/network/NetworkService.hpp"

int main()
{
    core::ecs::Registry registry;
    Game game(registry);
    game.run();
    return 0;
}

