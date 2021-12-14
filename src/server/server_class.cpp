#include "include/server_class.hpp"

Server::Server(int port, std::string mailSpoolDir)
{
    _buf = 1024;
    _port = port;
    _reuseVal = 1;
    _newSocket = -1;
    _createSocket = 0;
    _abortRequested = 0;
    _mailSpoolDir = mailSpoolDir;
}

void Server::StartServer()
{
    if(!InitSocket())
    {
        std::cerr << "InitSocket" << std::endl;
        throw "Socket initialization."; // TODO: exception class
    }
    if(!InitConnection())
    {
        std::cerr << "InitConnection" << std::endl;
        throw "Socket connection failed."; // TODO: exception class
    }
    ClientConnection();
}

bool Server::InitSocket()
{
    int reuseVal = 1;

    /*
    if (signal(SIGINT, SignalHandler) == SIG_ERR)
    {
        std::cerr << "Signal can not be registered." << std::endl;
        return EXIT_FAILURE;
    }
    */

    if((_createSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Socket Error." << std::endl;
        return EXIT_FAILURE;
    }

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

    return true;
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
    
    if(listen(_createSocket, 5) == -1)
    {
        std::cerr << "Listening error" << std::endl;
        return EXIT_FAILURE;
    }
    
    return true;
}

bool Server::SetCommand(ActionType action)
{
    Message msg;
    switch (action)
    {
    case SEND:
        UserCommand = new Send(msg);
        break;
    case LIST:
        /* code */
        break;
    case READ:
        /* code */
        break;
    case DEL:
        /* code */
        break;
    case QUIT:
        /* code */
        break;
    default:
        return false;
        break;
    }

    return true;
    // new message
}

void Server::ClientConnection()
{
    while (!_abortRequested)
    {
        std::cout << "Waiting for connections..." << std::endl;

        _addressLength = sizeof(struct sockaddr_in);
        if ((_newSocket = accept(_createSocket, (struct sockaddr *)&_cliaddress, &_addressLength)) == -1)
        {
            if(_abortRequested)
                std::cerr << "Accept error after aborted." << std::endl;
            else
                std::cerr << "Accept error." << std::endl;

            break;
        }
        
        std::cout << "Client connected from " << inet_ntoa(_cliaddress.sin_addr) << ": " << ntohs(_cliaddress.sin_port) << "..." << std::endl;
        ClientComm(&_newSocket);
        _newSocket = -1;
    }
}

void Server::ClientComm(void *data)
{
    int size;
    char buffer[_buf];
    int *currentSocket = (int *)data;

    strcpy(buffer, "Welcome to myserver!\r\nPlease enter your commands...\r\n");
    if (send(*currentSocket, buffer, strlen(buffer), 0) == -1)
    {
        std::cerr << "Send failed." << std::endl;
        throw "Send failed.";
    }

    do
    {
        size = recv(*currentSocket, buffer, _buf - 1, 0);
        if (size == -1)
        {
            if (_abortRequested)
                std::cerr << "recv error after aborted." << std::endl;

            else
                std::cerr << "recv error." << std::endl;

            break;
        }

        if (size == 0)
        {
            std::cout << "Client closed remote socket." << std::endl;
            break;
        }

        if (buffer[size - 2] == '\r' && buffer[size - 1] == '\n')
        {
            size -= 2;
        }
        else if (buffer[size - 1] == '\n')
        {
            --size;
        }

        buffer[size] = '\0';
        std::cout << "Message received: " << buffer << std::endl;

        if (strcmp(buffer, "SEND") == 0)
        {
            if(!SetCommand(SEND))
                throw "SEND Obj init failed";
            
            UserCommand->Action();
        } 
        else if (strcmp(buffer, "LIST") == 0)
        {
            if(!SetCommand(LIST))
                throw "LIST Obj init failed";
            
            UserCommand->Action();
        } 
        else if (strcmp(buffer, "READ") == 0)
        {
            if(!SetCommand(READ))
                throw "READ Obj init failed";
            
            UserCommand->Action();
        }
        else if (strcmp(buffer, "DEL") == 0)
        {
            if(!SetCommand(DEL))
                throw "DEL Obj init failed";
            
            UserCommand->Action();
        }
        else if (strcmp(buffer, "QUIT") == 0)
        {
            /* code */
        }
        

        if (send(*currentSocket, "OK", 3, 0) == -1)
        {
            std::cerr << "Send answer failed." << std::endl;
            throw "Send answer failed.";
        }

    } while (strcmp(buffer, "quit") != 0 && !_abortRequested);
}

void Server::CloseSockets(int socket)
{
    if (socket != -1)
    {
        if (shutdown(socket, SHUT_RDWR) == -1)
            std::cerr << "Shutdown socket." << std::endl;

        if (close(socket) == -1)
            std::cerr << "Close creatsockete_socket." << std::endl;

        socket = -1;
    }
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