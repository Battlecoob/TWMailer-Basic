#include <iostream>
#include <vector>

#include "message_class.hpp"

class User
{
private:
    std::string _username;
    std::vector<Message> _messages;

public:
    void SetName(std::string name) { _username = name; }

    // const Message GetMessageById(int i);
    const std::string GetName() { return _username; }
    const std::vector<Message> GetMessages() { return _messages; }

    void AddInbox(Message msg);
};