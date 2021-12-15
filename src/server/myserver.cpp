#include <stdio.h>
#include <dirent.h>

#include "include/server_class.hpp"


int main(int argc, char* argv[]) {
    std::cout << "myserver.cpp started..." << std::endl;
    
    Database db;

    int port = 6543;
    std::string path = "../database/";

    if(argc < 3) {
        std::cout<<"No or too few arguments provided"<<std::endl;
        std::cout<<"Using standard values instead"<<std::endl;
        path += "my_mail_db";
    }
    else if(opendir((path+argv[2]).c_str()) == NULL) {
        std::cout<<"Directory " << argv[2] << " does not exist"<<std::endl;
        return EXIT_FAILURE;
    }
    else {
        port = std::stoi(argv[1]);
        path += argv[2];
    }

    Server server(port, path, &db);

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