#include <iostream>

class Message
{
private:
    std::string _sender, _subject, _receiver, _messageText, _messageCount;

public:
    std::string GetSender()     { return _sender; }
    std::string GetSubject()    { return _subject; }
    std::string GetText()       { return _messageText; }
    std::string GetReceiver()   { return _receiver; }
    std::string GetCount()      { return _messageCount; }

    void SetSender(std::string sender)              { _sender = sender; }
    void SetSubject(std::string subject)            { _subject = subject; }
    void SetReceiver(std::string receiver)          { _receiver = receiver; }
    void SetMessageText(std::string messageText)    { _messageText = messageText; }
    void SetMessageCount(std::string messageCount)  { _messageCount = messageCount; }
};

