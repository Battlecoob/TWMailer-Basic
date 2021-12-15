#include "include/client_class.hpp"

Client::Client(/* args */)
{
}

Client::~Client()
{
}

bool Client::createConnection() {

    struct sockaddr_in address;

    if ((_create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Socket Error." << std::endl;
        return false;
    }

    memset(&address, 0, sizeof(address)); // init storage with 0
    address.sin_family = AF_INET;         // IPv4

    address.sin_port = htons(_port);
    inet_aton("127.0.0.1", &address.sin_addr);
   
   if (connect(_create_socket,
               (struct sockaddr *)&address,
               sizeof(address)) == -1)
   {
        std::cerr << "Connect error - no server available" << std::endl;
        return false;
   }

    std::cout<<"Connection established"<<std::endl;
    return true;
}

bool Client::sendLine() {
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

bool Client::recvLine() {

         _size = recv(_create_socket, _buffer,_buf - 1, 0);
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

void Client::SEND() {

   bool end = false;

   std::cout<<">>Username:";
   bool nextline = sendLine();
   nextline = recvLine();
   if(nextline) {
   std::cout<<">>Reciever:";
   nextline = sendLine();
   nextline = recvLine();
   }
   if(nextline) {
   std::cout<<">>Subject:";
   nextline = sendLine();
   nextline = recvLine();
   }
   if(nextline) {
   std::cout<<">>Message:";
   nextline = sendLine();
   nextline = recvLine();
   }
   // exclude the do-while-loop for non multiline messages
   do
   {
      std::cout<<">>Continue Message:";
      nextline = sendLine();
      if(strcmp(".", _buffer) == 0) {
         end = true;
      }
      nextline = recvLine();
   } while(!end);
   printf("<< %s\n", _buffer);
   
}
void Client::READ() {

   std::cout<<">>Username:";
   bool nextline = sendLine();
   nextline = recvLine();
   if(nextline) {
   std::cout<<">>Message Number:";
   nextline = sendLine();
   nextline = recvLine();
   }

   /*
   //ZEILE für ZEILE SEND
   printf(RESPONSE);
   */

   
}
void Client::LIST() {

   std::cout<<">>Username:";
   bool nextline = sendLine();
   nextline = recvLine();


   /*
   int count = _buffer;
   for (int i = 0; i < count; i++)

   //SERVER response:
   count
   für jede nachricht ID; Content
   {
      nextline = recvLine();
   }
   */
   
}
void Client::HELP() {
   std::cout<<"helping..."<<std::endl;
   std::cout<<"List valid commands when finished"<<std::endl;
}
void Client::DEL() {
   std::cout<<">>Username:";
   bool nextline = sendLine();
   nextline = recvLine();
   if(nextline) {
   std::cout<<">>Message Number:";
   nextline = sendLine();
   nextline = recvLine();
   }
}

int Client::readCommand() {

   if(strcmp(_buffer, "SEND") == 0) {
      return _send;
   }
   else if(strcmp(_buffer, "LIST") == 0) {
      return _list;
   }
   else if(strcmp(_buffer, "READ") == 0) {
      return _read;
   }
   else if(strcmp(_buffer, "DEL") == 0) {
      return _del;
   }
   else if(strcmp(_buffer, "HELP") == 0) {
      return _help;
   }
   if(strcmp(_buffer, "QUIT") == 0) {
      return _quit;
   }
   else{
      return -1;
   }
}

void Client::executeCommand(int execute) {

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
   case _help:
      HELP();
      break;
   case _quit:
      break;   
   default:
      printf("Invalid command! Type 'HELP' for a list of valid commands\n");
      break;
   }
}

void Client::waitForNextCommand() {
    _size = recv(_create_socket, _buffer, _buf - 1, 0);
    int command = -1;
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
      command = readCommand();
      sendLine();
      executeCommand(command);

   } while (command != _quit);
}

bool Client::clearConnection() {
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