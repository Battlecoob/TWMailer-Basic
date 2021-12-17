#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "/home/battlecoob/dev/TWMailer-Basic/src/share/functions.hpp" // zu dumm fuer var pfad

enum Command
{
    _send,
    _list,
    _read,
    _del,
    _help,
    _quit
};

class Client
{
private:
    int _buf = 1024;
    int _port = 6543;
    int _create_socket;
    char _buffer[1024];
    int _size;
    std::string _ip = "127.0.0.1";
    std::string _bufferstring;

public:
    Client(std::string ip, int port);

    Command readCommand();
    void executeCommand(Command execute);

    void readArguments();
    // reads arguments and wrapps them into the class structure
    bool createConnection();
    // connects to server (websocket, ports, etc...)
    bool clearConnection();
    // disconnects from server (websocket, ports, etc...)
    void waitForNextCommand();
    // loop that waits for user input, starts other method depending on input

    void SEND();
    void LIST();
    void READ();
    void DEL();

    bool sendLine();
    bool recvLine();
};
