#include <iostream>
#include "command_class.hpp"

class Client
{
private:
    /* data */
public:
    Client(/* args */);
    ~Client();

    void readArguments(/* args */);
    //reads arguments and wrapps them into the class structure

    void createConnection(/* args */);
    //connects to server (websocket, ports, etc...)

    void clearConnection(/* args */);
    //disconnects from server (websocket, ports, etc...)

    void waitForNextCommand(/* args */);
    //loop that waits for user input, starts other method depending on input

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
