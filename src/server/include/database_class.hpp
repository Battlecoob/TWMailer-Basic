#include <vector>
#include <iostream>
#include <sys/stat.h> // mkdir
#include <sys/types.h>
#include <bits/stdc++.h>

// #ifndef BOOST_ALL_DYN_LINK
// #   define BOOST_ALL_DYN_LINK
// #endif 
// #include <boost/filesystem.hpp>


#include "user_class.hpp"

class Database
{
private:
    std::string _directory;
    std::vector<User> _users;

public:
    Database();

    const User GetUserByName();
    const std::string GetDirectory() {return _directory; }
    const std::vector<User> GetUsers() { return _users; }

    const bool IsNewUser(User user);

    void AddUser(User user);
    void SetDirectory(std::string dir) { _directory = dir; }

    // bool SafeMessage(User user);
    // bool GetMessage(User user, /* unique identifier */);
    // bool DeleteMessage(User user, /* unique identifier */);
};
