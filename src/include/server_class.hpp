#include <unistd.h>     // close()
#include <csignal>      // signal(), raise(), ...
#include <iostream>
#include <string.h>     // memset()
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "command_class.hpp"

// enum Response
// {
//     ERR = 0,
//     OK
// };

class Server
{
private:
    int _buf;
    int _port;
    int _reuseVal;
    // Response _serverResponse;
    
    socklen_t _addressLength;
    std::string _mailSpoolDir;
    
    struct sockaddr_in _address;
    struct sockaddr_in _cliaddress;
    
    static int _newSocket;
    static int _createSocket;
    static int _abortRequested;

public:
    Command _userInput;
    
    Server(int port, std::string mailSpoolDir);
    
    int ReceiveMsg();

    bool InitSocket();
    bool StartServer();
    bool InitConnection();
    bool ClientConnection();

    void ClientComm();
    void SendWelcome();
    static void SignalHandler(int sig);
    void CloseSockets(int socket);
};