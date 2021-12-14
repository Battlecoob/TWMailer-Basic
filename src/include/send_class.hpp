#include "command_class.hpp"

class Send : public Command
{
// private:
//     Message _message;

public:
    Send(Message msg);

    bool Action();      // safes to db
    void UserInput();   // loop until user inputs all neccesary info
};