#include <iostream>

// enum ActionType
// {
//     SEND,
//     LIST,
//     READ,
//     DEL,
//     QUIT
// };

class Command // die einzelnen commands sollen inherited werden
{
private:

public:
    Command();
    ~Command();

    virtual const void UserInput();
};
