#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

class Client
{
private:
    int _buf = 1024;
    int _port = 6543;
    int _create_socket;
    char _buffer[1024];
    int _size;
    std::string _bufferstring;

    enum Command { _send,
        _list,
        _read,
        _del,
        _help,
        _quit};


public:
    Client(/* args */);
    ~Client();

    void readArguments(/* args */);
    //reads arguments and wrapps them into the class structure

    bool createConnection(/* args */);
    //connects to server (websocket, ports, etc...)

    bool clearConnection(/* args */);
    //disconnects from server (websocket, ports, etc...)

    void waitForNextCommand(/* args */);
    //loop that waits for user input, starts other method depending on input

    int readCommand();
    void executeCommand(int execute);

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
