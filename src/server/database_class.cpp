#include "include/database_class.hpp"

Database::Database()
{
    _directory = "../TWMailer-Basic/database/"; // /home/battlecoob/dev/TWMailer-Basic/database

    // fill users vector
    using recursive_directory_iterator = std::experimental::filesystem::recursive_directory_iterator;
    
    // for (const auto& dirEntry : recursive_directory_iterator(_directory))
    //     std::cout << dirEntry << std::endl;
}

/*
const User Database::GetUserByName(User tmpuser)
{
    for (int c = 0; c < (int)_users.size(); c++)
    {
        if (_users[c].GetName() == tmpuser.GetName())
            return _users[c];
    }
}
*/

const int Database::GetUserPositionInVector(User tmpuser)
{
    for (int c = 0; c < (int)_users.size(); c++)
    {
        if (_users[c].GetName() == tmpuser.GetName())
            return c;
    }
    return -1;
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

bool Database::AddUser(User user, std::string mailSpoolDir)
{
    // add to user vector
    user.UpdateMsgCounter();
    _users.push_back(user);

    std::cout << "Msg counter: " << user.GetMsgCounter() << std::endl;

    char resolved_path[PATH_MAX];
    realpath((_directory + mailSpoolDir + user.GetName()).c_str(), resolved_path);

    if(mkdir(resolved_path, 0777) == -1){
        std::cerr << "Error :  " << strerror(errno) << std::endl;
        return false;
    }

    std::cout << "Directory created." << std::endl;
    return true;
}

bool Database::AddMessage(User tmpuser, std::string mailSpoolDir)
{
    int positionInVec = GetUserPositionInVector(tmpuser);
    if(positionInVec != -1)
        EXIT_FAILURE;

    std::string userDir =  (_directory + mailSpoolDir + _users[positionInVec].GetName() + "/").c_str();
    
    auto filename = _users[positionInVec].GetMsgCounter();

    // create file -> msg counter from folder name
    std::ofstream File(userDir + std::to_string(filename));

    // write to file, sender, receiver, subj, text
    auto userMsg = _users[positionInVec].GetMessages();

    File    << userMsg[_users[positionInVec].GetMsgCounter()-1].GetSender() << "\n"
            << userMsg[_users[positionInVec].GetMsgCounter()-1].GetReceiver() << "\n"
            << userMsg[_users[positionInVec].GetMsgCounter()-1].GetSubject() << "\n"
            << userMsg[_users[positionInVec].GetMsgCounter()-1].GetText();
    
    File.close();
    return false;
}