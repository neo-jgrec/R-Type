#include "Server.hpp"

int main()
{
    Server server("0.0.0.0", 12345);

    server.update();
}
