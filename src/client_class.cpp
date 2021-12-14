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
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
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
   Nicht fertig SERVER antwort unklar 
   printf(RESPONSE);
   */

   
}
void Client::LIST() {

   std::cout<<">>Username:";
   bool nextline = sendLine();
   nextline = recvLine();


   /*
   Nicht fertig SERVER antwort unklar 
   int count = 3;
   for (int i = 0; i < count; i++)
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
      SEND();
      return _send;
   }
   else if(strcmp(_buffer, "LIST") == 0) {
      LIST();
      return _list;
   }
   else if(strcmp(_buffer, "READ") == 0) {
      READ();
      return _read;
   }
   else if(strcmp(_buffer, "DEL") == 0) {
      DEL();
      return _del;
   }
   else if(strcmp(_buffer, "HELP") == 0) {
      HELP();
      return _help;
   }
   if(strcmp(_buffer, "QUIT") == 0) {
      return _quit;
   }
   else{
      printf("Invalid command! Type 'HELP' for a list of valid commands\n");
      return -1;
   }
}

void Client::waitForNextCommand() {
    int isQuit;
    _size = recv(_create_socket, _buffer, _buf - 1, 0);
    int command = 0;
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
   } while (command != _quit);
}

bool Client::clearConnection() {
    if (_create_socket != -1)
    {
        if (shutdown(_create_socket, SHUT_RDWR) == -1)
        {
        // invalid in case the server is gone already
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