#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

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
    ~Client();

    void readArguments(/* args */);
    //reads arguments and wrapps them into the class structure

    bool createConnection(/* args */);
    //connects to server (websocket, ports, etc...)

    bool clearConnection(/* args */);
    //disconnects from server (websocket, ports, etc...)

    void waitForNextCommand(/* args */);
    //loop that waits for user input, starts other method depending on input

    Command readCommand();
    void executeCommand(Command execute);

    bool sendLine();
    bool recvLine();

    void SEND(/* args */);

    void LIST(/* args */);

    void READ(/* args */);

    void DEL(/* args */);

    void HELP(/* args */);
    //lists the valid commands

    //void QUIT(); ???

    //void submit(/* args */); ???
    //send message to server (maybe already done in SEND,LIST.....)



};

