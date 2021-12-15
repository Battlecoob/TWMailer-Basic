#include <unistd.h>     // close()
#include <csignal>      // signal(), raise(), ...
#include <iostream>
#include <string.h>     // memset()
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// #include "command_class.hpp"
#include "database_class.hpp"
#include "../../share/functions.hpp"

enum ActionType
{
    SEND,
    LIST,
    READ,
    DEL,
    QUIT
};

class Server
{
private:
    int _buf;
    int _port;
    int _reuseVal;
    int _newSocket;
    int _createSocket;
    int _abortRequested;

    Database _db;
    User _tmpUser;
    Message _tmpMsg;
    ActionType _action;
    
    socklen_t _addressLength;
    std::string _mailSpoolDir;
    
    struct sockaddr_in _address;
    struct sockaddr_in _cliaddress;
    
public:
    Server(int port, std::string mailSpoolDir, Database *db);
    
    // Command UserCommand;
    
    void SetAction(ActionType act) { _action = act; } // braucht man vlt gar nicht

    bool InitSocket();
    bool InitConnection(); 
    // bool SetCommand(ActionType action);

    void StartServer();
    void ClientConnection();
    void ClientComm(void *data);
    void SignalHandler(int sig);
    void CloseSockets(int socket);

    bool Send();
    bool List();
    bool Read();
    bool Delete();
};