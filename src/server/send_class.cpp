#include "include/send_class.hpp"

Send::Send(Message msg) : Command(msg) {}

bool Send::Action()
{
    std::cout << "IM SEND" << std::endl;
    return true;
}

void Send::UserInput(){}