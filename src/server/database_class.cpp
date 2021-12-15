#include "include/database_class.hpp"

Database::Database()
{
    // char resolved_path[PATH_MAX];
    _directory = "../TWMailer-Basic/database/"; // /home/battlecoob/dev/TWMailer-Basic/database
    // realpath(_directory.c_str(), resolved_path);
    // _directory = resolved_path;
    // fill users vector
}

const User Database::GetUserByName()
{
    return _users[0]; // FALSCH, nur dass die Warnings wegsind
}

const bool Database::IsNewUser(User user)
{
    for (int c = 0; c < (int)_users.size(); c++)
    {
        if(_users[c].GetName() == user.GetName())
            return false;
    }
    return true;
}

void Database::AddUser(User user)
{
    _users.push_back(user); // add to user vector
    //create directory
    //boost::filesystem::create_directories(_directory + user.GetName());
    // const char* tmpStr = (_directory + "/" + user.GetName()).c_str();
    // std::cout << tmpStr << std::endl;
    char resolved_path[PATH_MAX];
    realpath((_directory + user.GetName() ).c_str(), resolved_path);
    std::cout << resolved_path << std::endl;
    if(mkdir(resolved_path, 0777) == -1)
        std::cerr << "Error :  " << strerror(errno) << std::endl;
}