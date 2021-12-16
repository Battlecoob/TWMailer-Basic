#include <iostream>
#include <vector>

#include "message_class.hpp"

class User
{
private:
    int _msgCount;
    std::string _username;
    std::vector<Message> _messages;

public:
    void SetName(std::string name) { _username = name; }

    // const Message GetMessageById(int i);
    const int GetMsgCounter() { return _msgCount; }
    const std::string GetName() { return _username; }
    const std::vector<Message> GetMessages() { return _messages; }

    void AddInbox(Message msg);
    void UpdateMsgCounter() { _msgCount = _messages.size(); }
};