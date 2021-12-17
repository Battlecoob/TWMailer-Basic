#include <stdio.h>
#include <dirent.h>

#include "include/server_class.hpp"

int main(int argc, char *argv[])
{
    int port = 6543; // funktioniert nur mit diesem port
    std::string path = "";

    char realDir[PATH_MAX];
    realpath("../TWMailer-Basic/database/", realDir);
    std::string dir = realDir;
    dir += "/";

    // add to doku
    if (argc < 3)
    {
        std::cout << "None or too few arguments provided" << std::endl;
        std::cout << "Using standard values instead" << std::endl;
        path += "my_mail_db";
    }
    else
    {
        if (opendir((dir + argv[2]).c_str()) == NULL)
        {
            std::cerr << "Directory doesnt exist." << std::endl;
            return EXIT_FAILURE;
            // files arent correctly safed, if new mail spool dir is created :(
            /*
            if(mkdir((dir + argv[2]).c_str(), 0777) == -1)
            {
                std::cerr << "Error :  " << strerror(errno) << std::endl;
                return EXIT_FAILURE;
            } 
            else
                std::cout << "Directory: " << dir + argv[2] << " created." << std::endl;
            */
        }
        port = std::stoi(argv[1]);
        path += argv[2];
    }
    

    Database db;
    Server server(port, path, &db);
    db.SetDirectory(path);

    try
    {
        server.StartServer();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...) // currently not working correctly -> throw exception class  
    {
        std::cerr << "Thrown error." << std::endl;
    }

    std::cout << "Closed Server..." << std::endl;
    return 0;
}