#include <iostream>
#include <vector>

#include "message_class.hpp"

class User
{
private:
    std::string _username;
    std::vector<Message> _recMessages;
    std::vector<Message> _sentMessages;

public:
    void SetName(std::string name) { _username = name; }

    const std::string GetName() { return _username; }
    const std::vector<Message> GetReceivedMessages() { return _recMessages; }
    const std::vector<Message> GetSentMessages() { return _sentMessages; }

    void AddInbox(Message msg);
    void AddSent(Message msg);
};