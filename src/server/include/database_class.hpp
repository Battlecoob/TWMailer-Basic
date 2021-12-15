#include <iostream>
#include <vector>

#include "user_class.hpp"

class Database
{
private:
    std::string _directory;
    std::vector<User> _users;

public:
    const User GetUserByName();
    const std::string GetDirectory() {return _directory; }
    const std::vector<User> GetUsers() { return _users; }

    void SetDirectory(std::string dir) { _directory = dir; }
    void AddUser(User user) { _users.push_back(user); }

    // bool SafeMessage(User user);
    // bool GetMessage(User user, /* unique identifier */);
    // bool DeleteMessage(User user, /* unique identifier */);
};
