#include "include/user_class.hpp"

User::User()
{
    _msgCount = 0;
}

void User::AddMessage(Message msg)
{
    _messages.push_back(msg);
    UpdateMsgCounter();
}