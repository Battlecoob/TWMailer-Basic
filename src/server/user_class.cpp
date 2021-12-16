#include "include/user_class.hpp"

User::User()
{
    _msgCount = 0;
}

void User::AddMessage(Message msg)
{
    _msgCount++;
    msg.SetId(_msgCount);
    _messages.push_back(msg);
}