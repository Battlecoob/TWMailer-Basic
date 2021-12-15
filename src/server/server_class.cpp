#include "include/server_class.hpp"

Server::Server(int port, std::string mailSpoolDir, Database *db)
{
    _db = *db;
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
    if (send(_newSocket, buffer, strlen(buffer), 0) == -1)
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
            if (send(*currentSocket, "OK", 3, 0) == -1)
            {
                std::cerr << "Send answer failed." << std::endl;
                throw "Send answer failed.";
            }
            
            Send();
        }

        else if (strcmp(buffer, "LIST") == 0)
            List();

        else if (strcmp(buffer, "READ") == 0)
            Read();

        else if (strcmp(buffer, "DEL") == 0)
            Delete();

        else if (strcmp(buffer, "QUIT") == 0)
            strcpy(buffer, "quit");

        else
        {
            std::cout << "no compare" << std::endl;
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

void Server::Send()
{
    printf("in send\n");

    int size;
    char buffer[_buf];

    bool period = false;
    bool nameSet = false;
    bool recSet = false;
    bool subjSet = false;
    bool msgSet = false;

    do
    {
        // wait for input
        size = recv(_newSocket, buffer, _buf - 1, 0);
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

        if (strcmp(buffer, ".") == 0) // end of command
        {
            period = true;
            break;
        }

        std::cout << "Message received: " << buffer << std::endl;
        if(nameSet == false)
        {
            if(SecureInput(buffer)/* && (int)strlen(buffer) <= 8*/)
            {
                _tmpMsg.SetSender(buffer);
                nameSet = true;
                //send ok
                if (send(_newSocket, "OK", 3, 0) == -1)
                {
                    std::cerr << "Send answer failed." << std::endl;
                    throw "Send answer failed.";
                }
            }
            else
            {
                if (send(_newSocket, "ERR", 4, 0) == -1)
                {
                    std::cerr << "Send answer failed." << std::endl;
                    throw "Send answer failed.";
                }
            }
        }
        else if(!recSet)
        {
            if(SecureInput(buffer) && (int)strlen(buffer) <= 8)
            {
                _tmpUser.SetName(buffer);
                _tmpMsg.SetReceiver(buffer);
                recSet = true;
                //send ok
                if (send(_newSocket, "OK", 3, 0) == -1)
                {
                    std::cerr << "Send answer failed." << std::endl;
                    throw "Send answer failed.";
                }
            }
            else
            {
                if (send(_newSocket, "ERR", 4, 0) == -1)
                {
                    std::cerr << "Send answer failed." << std::endl;
                    throw "Send answer failed.";
                }
            }
        }
        else if(!subjSet)
        {
            if (strlen(buffer) < 81)
            {
                _tmpMsg.SetSubject(buffer);
                subjSet = true;
                //send ok
                if (send(_newSocket, "OK", 3, 0) == -1)
                {
                    std::cerr << "Send answer failed." << std::endl;
                    throw "Send answer failed.";
                }
            }
            else
            {
                /* errorhandling */
            }
        }
        else if (!msgSet)
        {
            if(!period)
            {
                _tmpMsg.AppendMessageText(buffer);
                //send ok
                if (send(_newSocket, "OK", 3, 0) == -1)
                {
                    std::cerr << "Send answer failed." << std::endl;
                    throw "Send answer failed.";
                }
            }
            else
                msgSet = true; // eig unnoetig aber egal
        }

    } while (!period);
    _tmpUser.AddInbox(_tmpMsg); // user + msg soll dann der db uebergeben werden
    std::cout << _tmpUser.GetName() << _tmpMsg.GetSender() << _tmpMsg.GetReceiver() << _tmpMsg.GetSubject() << _tmpMsg.GetText() << std::endl;
}

void Server::List() 
{
}

void Server::Read() 
{
}

void Server::Delete() 
{
}