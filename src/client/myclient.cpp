#include <stdio.h>
#include "include/client_class.hpp"

int main() {
    printf("Hello Client\n");

    // create client_class obj
    Client* myClient = new Client();

    //start read arguments method

    //start create connection method
    if(myClient->createConnection()){
        myClient->waitForNextCommand();
    }

    delete myClient;

    return 0;
}