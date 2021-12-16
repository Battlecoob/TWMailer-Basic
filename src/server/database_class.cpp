#include "include/database_class.hpp"

Database::Database()
{
    _directory = "/home/battlecoob/dev/TWMailer-Basic/database/"; // /home/battlecoob/dev/TWMailer-Basic/database
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

void Database::FillDB()
{
    // iterator for dir
    using directory_iterator = std::experimental::filesystem::directory_iterator;
    // iterate through dir
    for (const auto& dirEntry : directory_iterator(_directory))
    {        
        // add user to _users vector
        User newUser;
        newUser.SetName(dirEntry.path().stem().string());
        _users.push_back(newUser);
    }
    // iterate through users
    for (auto &u : _users)
    {
        // iterate through messages in user folder
        for (const auto& dirEntry : directory_iterator(_directory + u.GetName()))
        {
            std::string tmpstr;
            Message newMsg;
            // std::cout << dirEntry.path().string() << std::endl;

            std::ifstream msgFile;
            msgFile.open(dirEntry.path().string());
            
            // sender
            getline(msgFile, tmpstr);
            newMsg.SetSender(tmpstr);

            // receiver
            getline(msgFile, tmpstr);
            newMsg.SetReceiver(tmpstr);
            
            // subject
            getline(msgFile, tmpstr);
            newMsg.SetSubject(tmpstr);

            // message text
            while (getline(msgFile, tmpstr)) 
                newMsg.AppendMessageText(tmpstr);
            
            msgFile.close();
            u.AddMessage(newMsg);
        }
        // u.UpdateMsgCounter();
        std::cout << u.GetName() << u.GetMsgCounter() << std::endl;
    }
}

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

bool Database::AddUser(User user)
{
    // add to user vector
    _users.push_back(user);

    std::cout << "Msg counter: " << user.GetMsgCounter() << std::endl;

    char resolved_path[PATH_MAX];
    realpath((_directory + user.GetName()).c_str(), resolved_path);

    if(mkdir(resolved_path, 0777) == -1){
        std::cerr << "Error :  " << strerror(errno) << std::endl;
        return false;
    }

    std::cout << "Directory created." << std::endl;
    return true;
}

void Database::AddMessage(User tmpuser, Message tmpmsg)
{
    int positionInVec = GetUserPositionInVector(tmpuser);
    if(positionInVec != -1)
        EXIT_FAILURE;

    // add message to user
    _users[positionInVec].AddMessage(tmpmsg);
    std::string userDir =  (_directory + _users[positionInVec].GetName() + "/").c_str();
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
    std::cout << "file created" << std::endl;
}

const User Database::List(User tmpuser)
{
    int positionInVec = GetUserPositionInVector(tmpuser);
    if(positionInVec == -1)
    {
        User noUser;
        noUser.SetName("User doesnt exist");
        return noUser;
    }
    
    //test
    std::cout << positionInVec << std::endl;
    
    return _users[positionInVec];
}