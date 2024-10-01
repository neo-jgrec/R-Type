// #include "Server.hpp"
//
// int main()
// {
//     Server server("0.0.0.0", 12345);
//
//     server.update();
// }

#include "Server/Server.hpp"
#include "../../core/ecs/Registry/Registry.hpp"

int main()
{
    const core::ecs::Registry registry;
    Server server(registry);

    server.run();
    return 0;
}
