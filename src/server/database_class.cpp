#include "include/database_class.hpp"

Database::Database()
{
    // _directory = "../TWMailer-Basic/database/"; // /home/battlecoob/dev/TWMailer-Basic/database
    char realDir[PATH_MAX];
    realpath("../TWMailer-Basic/database/", realDir);
    _directory = realDir;
    _directory += "/";
}

void Database::FillDB()
{
    // iterator for dir
    using directory_iterator = std::experimental::filesystem::directory_iterator;
    // iterate through dir
    for (const auto &dirEntry : directory_iterator(_directory))
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
        for (const auto &dirEntry : directory_iterator(_directory + u.GetName()))
        {
            std::string tmpstr;
            Message newMsg;

            newMsg.SetId(std::stoi(dirEntry.path().filename().string()));

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
    }
}

const int Database::GetUserPositionInVector(User tmpuser)
{
    for (int c = 0; c < (int)_users.size(); c++)
    {
        if (_users[c].GetName() == tmpuser.GetName())
            return c;
    }
    return -1; // doesnt exist
}

bool Database::IsNewUser(User user)
{
    for (int c = 0; c < (int)_users.size(); c++)
    {
        if (_users[c].GetName() == user.GetName())
            return false;
    }
    return true;
}

bool Database::AddUser(User user)
{
    // add to user vector
    _users.push_back(user);

    if (mkdir((_directory + user.GetName()).c_str(), 0777) == -1)
    {
        std::cerr << "Error :  " << strerror(errno) << std::endl;
        return false;
    }

    std::cout << "Directory created" << std::endl;
    return true;
}

bool Database::AddMessage(User tmpuser, Message tmpmsg)
{
    int positionInVec = GetUserPositionInVector(tmpuser);
    if (positionInVec == -1)
    {
        std::cout << "user doesnt exist" << std::endl;
        return EXIT_FAILURE;
    }

    // add message to user
    _users[positionInVec].AddMessage(tmpmsg);

    std::string userDir = (_directory + _users[positionInVec].GetName() + "/").c_str();
    auto filename = _users[positionInVec].GetMsgCounter();

    // create file -> msg counter from folder name
    std::ofstream File(userDir + std::to_string(filename));

    // write to file, sender, receiver, subj, text
    auto userMsg = _users[positionInVec].GetMessages();

    File << userMsg[_users[positionInVec].GetMsgCounter() - 1].GetSender() << "\n"
         << userMsg[_users[positionInVec].GetMsgCounter() - 1].GetReceiver() << "\n"
         << userMsg[_users[positionInVec].GetMsgCounter() - 1].GetSubject() << "\n"
         << userMsg[_users[positionInVec].GetMsgCounter() - 1].GetText();

    File.close();
    std::cout << "File created" << std::endl;

    return true;
}

const User Database::List(User tmpuser)
{
    int positionInVec = GetUserPositionInVector(tmpuser);
    if (positionInVec == -1)
    {
        User noUser;
        noUser.SetName("User doesnt exist");
        return noUser;
    }

    return _users[positionInVec];
}

bool Database::Delete(std::string username, std::string file)
{
    std::string delFilePath;

    delFilePath = _directory + username + "/" + file;

    if (!std::experimental::filesystem::exists(delFilePath))
    {
        std::cout << "File not found" << std::endl;
        return false;
    }

    std::experimental::filesystem::remove(delFilePath);
    return true;
}

Message Database::Read(std::string username, std::string file)
{
    int index;
    User readUser;
    Message emptyMsg;
    std::string readFilePath;

    readUser.SetName(username);

    if ((index = GetUserPositionInVector(readUser)) == -1)
    {
        std::cout << "User doesnt exist" << std::endl;
        return emptyMsg;
    }

    readFilePath = _directory + username + "/" + file;
    std::cout << readFilePath << std::endl;

    if (!std::experimental::filesystem::exists(readFilePath))
    {
        std::cout << "File not found" << std::endl;
        return emptyMsg;
    }
    
    auto msgV = _users[index].GetMessages();
    for(auto m : msgV)
    {
        if(std::to_string(m.GetId()) == file)
            return m;
    }
    return emptyMsg;
}