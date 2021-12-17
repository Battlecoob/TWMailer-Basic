#include <stdio.h>
#include "include/client_class.hpp"

int main(int argc, char *argv[])
{
    int port = 6543;
    std::string ip = "127.0.0.1";

    // create client_class obj
    Client myClient(ip, port);

    // start read arguments method
    if (argc < 3)
    {
        std::cout << "None or too few arguments provided" << std::endl;
        std::cout << "Using standard values instead" << std::endl;
    }
    else
    {
        ip = argv[1];
        port = std::stoi(argv[2]);
    }

    // start create connection method
    if (myClient.createConnection())
        myClient.waitForNextCommand();

    myClient.clearConnection();

    return 0;
}