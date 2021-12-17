#include "include/user_class.hpp"

User::User()
{
    _msgCount = 0;
}

void User::AddMessage(Message msg)
{
    if (msg.GetId() < 0)
        msg.SetId(_msgCount + 1);

    while (msg.GetId() > _msgCount)
        _msgCount = msg.GetId();

    // funkioniert nicht gscheit, weil wenn eine nachricht geloescht wird, soll dieser vector eig leer sein
    _messages.push_back(msg);
}