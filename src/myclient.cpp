#include <stdio.h>
#include "include/client_class.hpp"

int main() {
    printf("Hello Client\n");

    // create client_class obj
    Client* myClient = new Client();

    //start read arguments method

    //start create connection method
    if(myClient->createConnection()){
        //start input/command method
    }

    //execute clear connection method


    return 0;
}