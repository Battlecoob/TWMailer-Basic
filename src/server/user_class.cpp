#include "include/user_class.hpp"

void User::AddInbox(Message msg)
{
    _recMessages.push_back(msg);
}

void User::AddSent(Message msg)
{
    _sentMessages.push_back(msg);
}