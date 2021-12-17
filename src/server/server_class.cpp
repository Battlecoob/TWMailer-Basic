#include "include/server_class.hpp"

Server::Server(int port, std::string mailSpoolDir, Database *db)
{
    _size = 0;
    _db = *db;
    _buf = buf;
    _port = port;
    _reuseVal = 1;
    _newSocket = -1;
    _createSocket = 0;
    _abortRequested = 0;
    _mailSpoolDir = mailSpoolDir + "/";

    _db.SetDirectory(_mailSpoolDir);
    _db.FillDB();
}

bool Server::recvLine()
{
    _size = recv(_newSocket, _buffer, _buf - 1, 0);
    if (_size == -1)
    {
        if (_abortRequested)
            std::cerr << "recv error after aborted." << std::endl;

        else
            std::cerr << "recv error." << std::endl;

        return false;
    }

    if (_size == 0)
    {
        std::cout << "Client closed remote socket." << std::endl;
        return false;
    }

    if (_buffer[_size - 2] == '\r' && _buffer[_size - 1] == '\n')
    {
        _size -= 2;
    }
    else if (_buffer[_size - 1] == '\n')
    {
        --_size;
    }

    _buffer[_size] = '\0';
    return true;
}

bool Server::InitSocket()
{
    int reuseVal = 1;

    if ((_createSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Socket Error." << std::endl;
        return EXIT_FAILURE;
    }

    if (setsockopt(_createSocket, SOL_SOCKET, SO_REUSEADDR, &reuseVal, sizeof(reuseVal)) == -1)
    {
        std::cerr << "Error while setting socket adress option." << std::endl;
        return EXIT_FAILURE;
    }

    if (setsockopt(_createSocket, SOL_SOCKET, SO_REUSEPORT, &reuseVal, sizeof(reuseVal)) == -1)
    {
        std::cerr << "Error while setting socket port option." << std::endl;
        return EXIT_FAILURE;
    }

    return true;
}

bool Server::InitConnection()
{
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);

    if (bind(_createSocket, (struct sockaddr *)&_address, sizeof(_address)) == -1)
    {
        std::cerr << "Bind error" << std::endl;
        return EXIT_FAILURE;
    }

    if (listen(_createSocket, 5) == -1)
    {
        std::cerr << "Listening error" << std::endl;
        return EXIT_FAILURE;
    }

    return true;
}

void Server::SendOK()
{
    if (send(_newSocket, "OK", 3, 0) == -1)
    {
        std::cerr << "Send answer failed." << std::endl;
        throw "Send answer failed.";
    }
}

void Server::SendERR()
{
    if (send(_newSocket, "ERR", 4, 0) == -1)
    {
        std::cerr << "Send answer failed." << std::endl;
        throw "Send answer failed.";
    }
}

void Server::StartServer()
{
    if (!InitSocket())
    {
        std::cerr << "InitSocket" << std::endl;
        throw "Socket initialization."; // TODO: exception class
    }
    if (!InitConnection())
    {
        std::cerr << "InitConnection" << std::endl;
        throw "Socket connection failed."; // TODO: exception class
    }
    ClientConnection();
}

void Server::ClientConnection()
{
    while (!_abortRequested)
    {
        std::cout << "Waiting for connections..." << std::endl;

        _addressLength = sizeof(struct sockaddr_in);
        if ((_newSocket = accept(_createSocket, (struct sockaddr *)&_cliaddress, &_addressLength)) == -1)
        {
            if (_abortRequested)
                std::cerr << "Accept error after aborted." << std::endl;
            else
                std::cerr << "Accept error." << std::endl;

            break;
        }

        std::cout << "Client connected from " << inet_ntoa(_cliaddress.sin_addr) << ": " << ntohs(_cliaddress.sin_port) << "..." << std::endl;
        ClientComm(&_newSocket);
        _newSocket = -1;
    }
}

void Server::ClientComm(void *data)
{

    strcpy(_buffer, "Welcome to myserver!\r\n");
    if (send(_newSocket, _buffer, strlen(_buffer), 0) == -1)
    {
        std::cerr << "Send failed." << std::endl;
        throw "Send failed.";
    }

    do
    {

        if (!recvLine())
            return;
        std::cout << "Message received: " << _buffer << std::endl;

        if (strcmp(_buffer, "SEND") == 0)
        {
            SendOK();
            Send();
        }
        else if (strcmp(_buffer, "LIST") == 0)
        {
            SendOK();
            List();
        }
        else if (strcmp(_buffer, "READ") == 0)
        {
            SendOK();
            Read();
        }
        else if (strcmp(_buffer, "DEL") == 0)
        {
            SendOK();
            Delete();
        }
        else if (strcmp(_buffer, "QUIT") == 0)
        {
            SendOK();
            strcpy(_buffer, "QUIT");
        }
        else
        {
            SendOK();
        }

    } while (strcmp(_buffer, "QUIT") != 0 && !_abortRequested);
}

void Server::CloseSockets(int socket)
{
    if (socket != -1)
    {
        if (shutdown(socket, SHUT_RDWR) == -1)
            std::cerr << "Shutdown socket." << std::endl;

        if (close(socket) == -1)
            std::cerr << "Close creatsockete_socket." << std::endl;

        socket = -1;
    }
}

void Server::Send()
{
    Message tmpMsg;
    bool period = false;
    bool nameSet = false;
    bool recSet = false;
    bool subjSet = false;
    bool msgSet = false;

    do
    {
        if (!recvLine())
            return; // EXITFailure

        if (strcmp(_buffer, ".") == 0) // end of command
        {
            period = true;
            SendOK();
        }

        std::cout << "Message received: " << _buffer << std::endl;
        if (nameSet == false)
        {
            if (SecureInput(_buffer) && (strlen(_buffer) <= 8 && strlen(_buffer) > 0) /* && (int)strlen(_buffer) <= 8*/)
            {
                tmpMsg.SetSender(_buffer);
                nameSet = true;
                SendOK();
            }
            else
                SendERR();
        }
        else if (!recSet)
        {
            if (SecureInput(_buffer) && (strlen(_buffer) <= 8 && strlen(_buffer) > 0))
            {
                _tmpUser.SetName(_buffer);
                tmpMsg.SetReceiver(_buffer);
                recSet = true;
                SendOK();
            }
            else
                SendERR();
        }
        else if (!subjSet)
        {
            if (strlen(_buffer) < 81)
            {
                tmpMsg.SetSubject(_buffer);
                subjSet = true;
                SendOK();
            }
            else
                SendERR();
        }
        else if (!msgSet)
        {
            if (!period)
            {
                tmpMsg.AppendMessageText(_buffer);
                SendOK();
            }
            else
                msgSet = true; // eig unnoetig aber egal
        }
    } while (!period);

    if (nameSet && recSet && subjSet && msgSet)
    {
        if (_db.IsNewUser(_tmpUser))
        {
            if (!_db.AddUser(_tmpUser))
                throw "User couldnt be created";
        }
        _db.AddMessage(_tmpUser, tmpMsg);
    } 
    else
        throw "Something went wrong";
}

void Server::List()
{

    // wait for input
    if (!recvLine())
        return;

    std::cout << "Message received: " << _buffer << std::endl;

    if (SecureInput(_buffer))
    {
        _tmpUser.SetName(_buffer);

        _tmpUser = _db.List(_tmpUser);

        // message counter
        auto userMsgVector = _tmpUser.GetMessages();
        int msgCount = (int)_tmpUser.GetMessages().size();
        std::string strMsgCount = std::to_string(msgCount);

        if (send(_newSocket, strMsgCount.c_str(), (int)strMsgCount.size() + 1, 0) == -1)
        {
            std::cerr << "Send answer failed." << std::endl;
            throw "Send answer failed.";
        }
        for (int c = 0; c < msgCount; c++)
        {
            std::cout << "test" << std::endl; // magic cout -> we dont know why, but if it doesnt exist, nothing is send on socket ...
            // id
            auto userMsg = userMsgVector[c];
            int id = userMsg.GetId();
            std::string strId = std::to_string(id);

            // subject
            std::string strSub = userMsg.GetSubject();
            std::string sendText = strId + ": " + strSub;

            if (send(_newSocket, sendText.c_str(), (int)sendText.size() + 1, 0) == -1)
            {
                std::cerr << "Send answer failed." << std::endl;
                throw "Send answer failed.";
            }
        }
    }
    return;
}

void Server::Read()
{
    Message m;
    std::string readUser, readFile, tmpstr;

    // user name
    if (!recvLine())
        return;

    std::cout << "Message received: " << _buffer << std::endl;
    if (SecureInput(_buffer))
    {
        readUser = _buffer;
        SendOK();
    }

    // file
    if (!recvLine())
        return;

    std::cout << "Message received: " << _buffer << std::endl;
    if (SecureInput(_buffer))
        readFile = _buffer;
    
    if((m = _db.Read(readUser, readFile)).GetId() == -1)
    {
        SendERR();
        return;    
    }

    SendOK();

    // message content
    tmpstr = "<Sender> " + m.GetSender();
    if (send(_newSocket, tmpstr.c_str(), (int)tmpstr.size() + 1, 0) == -1)
    {
        std::cerr << "Send answer failed." << std::endl;
        throw "Send answer failed.";
    }
    tmpstr = "<Receiver> " + m.GetReceiver();
    if (send(_newSocket, tmpstr.c_str(), (int)tmpstr.size() + 1, 0) == -1)
    {
        std::cerr << "Send answer failed." << std::endl;
        throw "Send answer failed.";
    }
    tmpstr = "<Subject> " + m.GetSubject();
    if (send(_newSocket, tmpstr.c_str(), (int)tmpstr.size() + 1, 0) == -1)
    {
        std::cerr << "Send answer failed." << std::endl;
        throw "Send answer failed.";
    }
    tmpstr = "<Message> " + m.GetText();
    if (send(_newSocket, tmpstr.c_str(), (int)tmpstr.size() + 1, 0) == -1)
    {
        std::cerr << "Send answer failed." << std::endl;
        throw "Send answer failed.";
    }
}

void Server::Delete()
{
    std::string delUser, delFile;

    // user name
    if (!recvLine())
        return;

    std::cout << "Message received: " << _buffer << std::endl;
    if (SecureInput(_buffer))
    {
        delUser = _buffer;
        SendOK();
    }

    // file
    if (!recvLine())
        return;

    std::cout << "Message received: " << _buffer << std::endl;
    if (SecureInput(_buffer))
        delFile = _buffer;

    // delete said file
    if (_db.Delete(delUser, delFile))
        SendOK();
    else
        SendERR();
}