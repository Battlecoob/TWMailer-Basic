#include <stdio.h>

#include "include/server_class.hpp"

#define PORT 6543
#define PATH "my_mail_db"

int main() {
    std::cout << "myserver.cpp started..." << std::endl;
    
    Database db;
    Server server(PORT, PATH, &db);

    try
    {
        server.StartServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(...) // currently not working correctly -> throw exception class
    {
        std::cerr << "Thrown error." << std::endl;
    }

    std::cout << "myserver.cpp terminated..." << std::endl;
    return 0;
}