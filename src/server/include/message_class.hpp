#include <iostream>

class Message
{
protected:
    int _id;
    std::string _sender, _subject, _receiver, _messageText;

public:
    Message();

    const int GetId() { return _id; }
    const std::string GetSender() { return _sender; }
    const std::string GetSubject() { return _subject; }
    const std::string GetText() { return _messageText; }
    const std::string GetReceiver() { return _receiver; }

    void SetId(int id) { _id = id; }
    void SetSender(std::string sender) { _sender = sender; }
    void SetSubject(std::string subject) { _subject = subject; }
    void SetReceiver(std::string receiver) { _receiver = receiver; }
    void AppendMessageText(std::string messageText) { _messageText += messageText; }
};