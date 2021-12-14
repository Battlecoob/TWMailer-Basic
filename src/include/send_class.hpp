#include "command_class.hpp"

class Send : public Command
{
public:
    bool Action();      // safes to db
    void UserInput();   // loop until user inputs all neccesary info
};