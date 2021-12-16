#include <vector>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
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

    void SetDirectory(std::string dir) { _directory += dir; }

    // const User GetUserByName(User tmpuser);
    const int GetUserPositionInVector(User tmpuser);
    const std::vector<User> GetUsers() { return _users; }
    const std::string GetDirectory() {return _directory; }

    void FillDB();

    const bool IsNewUser(User user);
    bool AddUser(User user/*, std::string mailSpoolDir*/);
    void AddMessage(User user, Message tmpmsg);

    // bool SafeMessage(User user);
    // bool GetMessage(User user, /* unique identifier */);
    // bool DeleteMessage(User user, /* unique identifier */);
};
