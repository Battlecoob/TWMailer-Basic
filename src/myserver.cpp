#include <stdio.h>

#include "include/server_class.hpp"

#define PORT 6543
#define PATH ""

int main() {
    std::cout << "myserver.cpp started..." << std::endl;
    
    Server server(PORT, PATH);
    
    try
    {
        server.StartServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << "myserver.cpp terminated..." << std::endl;
    return 0;
}