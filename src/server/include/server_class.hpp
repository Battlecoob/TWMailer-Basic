#include <unistd.h> // close()
#include <csignal>  // signal(), raise(), ...
#include <string.h> // memset()
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "database_class.hpp"
#include "../../share/functions.hpp"

#define buf 1024 // sorry aber ich weiss nicht wie ich char _buffer[] sonst im header schreiben kann


class Server
{
private:
    int _buf;
    int _size;
    int _port;
    int _reuseVal;
    int _newSocket;
    int _createSocket;
    int _abortRequested;

    char _buffer[buf];

    Database _db;
    User _tmpUser;

    socklen_t _addressLength;
    std::string _mailSpoolDir;

    struct sockaddr_in _address;
    struct sockaddr_in _cliaddress;

public:
    Server(int port, std::string mailSpoolDir, Database *db);

    const std::string GetMailDir() { return _mailSpoolDir; }

    bool recvLine();
    bool InitSocket();
    bool InitConnection();
    // bool SetCommand(ActionType action);

    void SendOK();
    void SendERR();
    void StartServer();
    void ClientConnection();
    void ClientComm(void *data);
    void CloseSockets(int socket);

    void Send();
    void List();
    void Read();
    void Delete();
};