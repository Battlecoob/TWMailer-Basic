#include "include/client_class.hpp"

Client::Client(std::string ip, int port)
{
   _ip = ip;
   _port = port;
}

bool Client::createConnection()
{

   struct sockaddr_in address;

   if ((_create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      std::cerr << "Socket Error." << std::endl;
      return false;
   }

   memset(&address, 0, sizeof(address)); // init storage with 0
   address.sin_family = AF_INET;         // IPv4

   address.sin_port = htons(_port);
   inet_aton(_ip.c_str(), &address.sin_addr);

   if (connect(_create_socket, (struct sockaddr *)&address, sizeof(address)) == -1)
   {
      std::cerr << "Connect error - no server available" << std::endl;
      return false;
   }

   std::cout << "Connection established" << std::endl;
   return true;
}

bool Client::clearConnection()
{
   if (_create_socket != -1)
   {
      if (shutdown(_create_socket, SHUT_RDWR) == -1)
      {
         perror("shutdown create_socket");
      }
      if (close(_create_socket) == -1)
      {
         perror("close create_socket");
      }
      _create_socket = -1;
   }

   return EXIT_SUCCESS;
}

void Client::SEND()
{
   bool end = false;
   bool nextline = false;

   while (!nextline)
   {
      std::cout << ">>Username:";
      nextline = sendLine();
      if (SecureInput(_buffer) && (strlen(_buffer) <= 8 && strlen(_buffer) > 0))
         nextline = true;
      else
         std::cout << "Username: Max. 8 characters (a-z, 0-9)." << std::endl;

      nextline = recvLine();
   }

   nextline = false;

   while (!nextline)
   {
      std::cout << ">>Reciever:";
      nextline = sendLine();
      if (SecureInput(_buffer) && (strlen(_buffer) <= 8 && strlen(_buffer) > 0))
         nextline = true;
      else
         std::cout << "Reciever: Max. 8 characters (a-z, 0-9)." << std::endl;

      nextline = recvLine();
   }

   if (nextline)
   {
      std::cout << ">>Subject:";
      nextline = sendLine();
      nextline = recvLine();
   }

   if (nextline)
   {
      std::cout << ">>Message:";
      nextline = sendLine();
      nextline = recvLine();
   }

   // exclude the do-while-loop for non multiline messages
   do
   {
      std::cout << ">>Continue Message:";
      nextline = sendLine();
      std::cout << _buffer << std::endl;
      
      if (strcmp(".", _buffer) == 0)
         end = true;

      nextline = recvLine();
   } while (!end);
}

void Client::LIST()
{

   int count;
   std::cout << ">>Username:";
   bool nextline = sendLine();
   if (nextline)
   {
      nextline = recvLine();
      std::cout << _buffer << std::endl;
      count = std::stoi(_buffer);
   }

   std::cout << count << std::endl;

   for (int i = 0; i < count; i++)
   {
      nextline = recvLine();
      std::cout << _buffer << std::endl;
   }
}

void Client::READ()
{

   std::cout << ">>Username:";
   bool nextline = sendLine();
   nextline = recvLine();
   if (nextline)
   {
      std::cout << ">>Message Number:";
      nextline = sendLine();
      nextline = recvLine(); // ok // err
   }
   // msg content
   if(nextline) // if ok
   {
      for (int i = 0; i < 4; i++)
      {
         nextline = recvLine();
         std::cout << _buffer << std::endl;
      }
   }
   else
      std::cout << "<< File not Found." << std::endl;
}

void Client::DEL()
{
   std::cout << ">>Username:";
   bool nextline = sendLine();
   nextline = recvLine();
   if (nextline)
   {
      std::cout << ">>Message Number:";
      nextline = sendLine();
      nextline = recvLine();
   }
}

Command Client::readCommand()
{

   if (strcmp(_buffer, "SEND") == 0)
      return _send;

   else if (strcmp(_buffer, "LIST") == 0)
      return _list;

   else if (strcmp(_buffer, "READ") == 0)
      return _read;

   else if (strcmp(_buffer, "DEL") == 0)
      return _del;

   if (strcmp(_buffer, "QUIT") == 0)
      return _quit;

   return _quit;
}

void Client::executeCommand(Command execute)
{

   switch (execute)
   {
   case _send:
      SEND();
      break;
   case _list:
      LIST();
      break;
   case _read:
      READ();
      break;
   case _del:
      DEL();
      break;
   case _quit:
      break;
   default:
      break;
   }
}

void Client::waitForNextCommand()
{
   _size = recv(_create_socket, _buffer, _buf - 1, 0);
   Command command;
   if (_size == -1)
   {
      perror("recv error");
   }
   else if (_size == 0)
   {
      printf("Server closed remote socket\n"); // ignore error
   }
   else
   {
      _buffer[_size] = '\0';
      printf("%s", _buffer); // ignore error
   }

   do
   {
      printf("Please enter your command!\n");
      printf(">> ");
      sendLine();
      command = readCommand();
      recvLine();
      executeCommand(command);
   } while (command != _quit);
}

bool Client::sendLine()
{
   if (fgets(_buffer, _buf, stdin) != NULL)
   {
      _size = strlen(_buffer);
      if (_buffer[_size - 2] == '\r' && _buffer[_size - 1] == '\n')
      {
         _size -= 2;
         _buffer[_size] = 0;
      }
      else if (_buffer[_size - 1] == '\n')
      {
         --_size;
         _buffer[_size] = 0;
      }

      if ((send(_create_socket, _buffer, _size, 0)) == -1)
      {
         perror("send error");
         return false;
      }
   }
   return true;
}

bool Client::recvLine()
{

   _size = recv(_create_socket, _buffer, _buf - 1, 0);
   if (_size == -1)
   {
      perror("recv error");
      return false;
   }
   else if (_size == 0)
   {
      printf("Server closed remote socket\n"); // ignore error
      return false;
   }
   else
   {
      _buffer[_size] = '\0';
      if (strcmp("ERR", _buffer) == 0)
      {
         fprintf(stderr, "<< Server error occured, abort\n");
         return false;
      }
   }
   return true;
}
