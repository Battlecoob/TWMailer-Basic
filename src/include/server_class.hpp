#include <iostream>
#include "command_class.hpp"

enum Response
{
    ERR = 0,
    OK
};

class Server
{
private:
    int _port;
    Response _serverResponse;
    std::string _mailSpoolDir;

public:
    Server(/* args */);
    ~Server();
};