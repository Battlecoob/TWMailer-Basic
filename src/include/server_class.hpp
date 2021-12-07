#include <unistd.h>     // close()
#include <csignal>      // signal(), raise(), ...
#include <iostream>
#include <string.h>     // memset()
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "command_class.hpp"

class Server
{
private:
    int _buf;
    int _port;
    int _reuseVal;
    
    static int _newSocket;
    static int _createSocket;
    static int _abortRequested;
    
    socklen_t _addressLength;
    std::string _mailSpoolDir;
    
    struct sockaddr_in _address;
    struct sockaddr_in _cliaddress;
    
public:
    Command _userInput;
    
    Server(int port, std::string mailSpoolDir);
    
    bool InitSocket(); //
    bool InitConnection(); //
    bool ClientConnection(); //

    void StartServer(); //
    void *ClientComm(void *data); //
    void CloseSockets(int socket); //
    static void SignalHandler(int sig); //
};