#include <iostream>

enum ActionType
{
    SEND,
    LIST,
    READ,
    DEL,
    QUIT
};

class Command // die einzelnen commands sollen vererbt werden
{
public:
    ActionType type;
    virtual int Action();
    virtual const void UserInput();
};