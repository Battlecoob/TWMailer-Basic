#include <vector>
#include <fstream>
#include <iostream>
#include <sys/stat.h> // mkdir
#include <sys/types.h>
#include <bits/stdc++.h>
#include <experimental/filesystem>

#include "user_class.hpp"

class Database
{
private:
    std::string _directory;
    std::vector<User> _users;

public:
    Database();

    void SetDirectory(std::string dir) { _directory += dir; }

    const int GetUserPositionInVector(User tmpuser);
    const std::vector<User> GetUsers() { return _users; }
    const std::string GetDirectory() { return _directory; }

    void FillDB();

    bool AddUser(User user);
    bool IsNewUser(User user);
    const User List(User user);
    bool AddMessage(User user, Message tmpmsg);
    bool Delete(std::string username, std::string file);
    Message Read(std::string username, std::string file); // not finished
};
