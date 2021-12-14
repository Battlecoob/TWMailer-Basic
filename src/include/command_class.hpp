#include <iostream>

#include "message_class.hpp"

class Command // die einzelnen commands sollen vererbt werden
{
protected:
    Message _message;

public:
    Command(Message msg);

    virtual bool Action();
    virtual void UserInput();
};