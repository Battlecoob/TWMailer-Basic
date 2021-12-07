#include "include/server_class.hpp"

Server::Server(int port, std::string mailSpoolDir)
{
    _buf = 1024;
    _port = port;
    _reuseVal = 1;
    _newSocket = -1;
    _createSocket = -1;
    _abortRequested = 0;
    _mailSpoolDir = mailSpoolDir;
}

bool Server::InitSocket()
{
    int reuseVal = 1;

    // signal handler
    
    if (signal(SIGINT, SignalHandler) == SIG_ERR) // expression must have class type but it has type "Server *"
    {
        std::cerr << "Signal can not be registered." << std::endl;
        return EXIT_FAILURE;
    }

    // create socket
    if((_createSocket = socket(AF_INET, SOCK_STREAM, 0)) != -1)
    {
        std::cerr << "Socket Error." << std::endl;
        return EXIT_FAILURE;
    }

    // set socket options
    if(setsockopt(_createSocket, SOL_SOCKET, SO_REUSEADDR, &reuseVal, sizeof(reuseVal)) == -1)
    {
        std::cerr << "Error while setting socket adress option." << std::endl;
        return EXIT_FAILURE;
    }

    if(setsockopt(_createSocket, SOL_SOCKET, SO_REUSEPORT, &reuseVal, sizeof(reuseVal)) == -1)
    {
        std::cerr << "Error while setting socket port option." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool Server::InitConnection()
{
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
   
    if (bind(_createSocket, (struct sockaddr *)&_address, sizeof(_address)) == -1)
    {
        std::cerr << "Bind error" << std::endl;
        return EXIT_FAILURE;
    }
    
    if(listen(_createSocket, 5) == -1) // muss 5?
    {
        std::cerr << "Listening error" << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

bool Server::ClientConnection()
{
}

void Server::SignalHandler(int sig)
{
    if (sig == SIGINT)
    {
        _abortRequested = 1;
        std::cerr << "Abort Requested." << std::endl;

        if (_newSocket != -1)
        {
            if (shutdown(_newSocket, SHUT_RDWR) == -1)
                std::cerr << "Shutdown _newSocket." << std::endl;

            if (close(_newSocket) == -1)
                std::cerr << "Close _newSocket." << std::endl;

            _newSocket = -1;
        }

        if (_createSocket != -1)
        {
            if (shutdown(_createSocket, SHUT_RDWR) == -1)
                std::cerr << "Shutdown _createSocket." << std::endl;

            if (close(_createSocket) == -1)
                std::cerr << "Close _createSocket." << std::endl;

            _createSocket = -1;
        }
    }
    else
        exit(sig);
}

void Server::CloseSockets(int socket)
{
    if (socket != -1)
    {
        if (shutdown(socket, SHUT_RDWR) == -1)
            perror("shutdown socket");

        if (close(socket) == -1)
            perror("close creatsockete_socket");

        socket = -1;
    }
}