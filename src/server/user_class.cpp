#include "include/user_class.hpp"

void User::AddInbox(Message msg)
{
    _messages.push_back(msg);
    UpdateMsgCounter();
}