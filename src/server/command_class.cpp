#include "include/command_class.hpp"

Command::Command(Message msg)
{
    _message = msg;
}

bool Command::Action()
{ 
    std::cout << "IM COMMAND" << std::endl;
    return false; 
}

void Command::UserInput(){}