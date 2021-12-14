#include <iostream>

class Message
{
protected:
public:
    std::string _sender, _subject, _receiver, _messageText;

    Message();

    std::string GetSender() { return _sender; }
    std::string GetSubject() { return _subject; }
    std::string GetText() { return _messageText; }
    std::string GetReceiver() { return _receiver; }

    void SetSender(std::string sender) { _sender = sender; }
    void SetSubject(std::string subject) { _subject = subject; }
    void SetReceiver(std::string receiver) { _receiver = receiver; }
    void SetMessageText(std::string messageText) { _messageText = messageText; }
};

