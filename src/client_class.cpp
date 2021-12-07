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

void Client::waitForNextCommand() {
    int isQuit;
    int size = recv(_create_socket, _buffer, _buf - 1, 0);
   if (size == -1)
   {
      perror("recv error");
   }
   else if (size == 0)
   {
      printf("Server closed remote socket\n"); // ignore error
   }
   else
   {
      _buffer[size] = '\0';
      printf("%s", _buffer); // ignore error
   }

   do
   {
      printf(">> ");
      if (fgets(_buffer, _buf, stdin) != NULL)
      {
         int size = strlen(_buffer);
         // remove new-line signs from string at the end
         if (_buffer[size - 2] == '\r' && _buffer[size - 1] == '\n')
         {
            size -= 2;
            _buffer[size] = 0;
         }
         else if (_buffer[size - 1] == '\n')
         {
            --size;
            _buffer[size] = 0;
         }
         isQuit = strcmp(_buffer, "quit") == 0;

         //////////////////////////////////////////////////////////////////////
         // SEND DATA
         // https://man7.org/linux/man-pages/man2/send.2.html
         // send will fail if connection is closed, but does not set
         // the error of send, but still the count of bytes sent
         if ((send(_create_socket, _buffer, size, 0)) == -1) 
         {
            // in case the server is gone offline we will still not enter
            // this part of code: see docs: https://linux.die.net/man/3/send
            // >> Successful completion of a call to send() does not guarantee 
            // >> delivery of the message. A return value of -1 indicates only 
            // >> locally-detected errors.
            // ... but
            // to check the connection before send is sense-less because
            // after checking the communication can fail (so we would need
            // to have 1 atomic operation to check...)
            perror("send error");
            break;
         }

         //////////////////////////////////////////////////////////////////////
         // RECEIVE FEEDBACK
         // consider: reconnect handling might be appropriate in somes cases
         //           How can we determine that the command sent was received 
         //           or not? 
         //           - Resend, might change state too often. 
         //           - Else a command might have been lost.
         //
         // solution 1: adding meta-data (unique command id) and check on the
         //             server if already processed.
         // solution 2: add an infrastructure component for messaging (broker)
         //
         size = recv(_create_socket, _buffer,_buf - 1, 0);
         if (size == -1)
         {
            perror("recv error");
            break;
         }
         else if (size == 0)
         {
            printf("Server closed remote socket\n"); // ignore error
            break;
         }
         else
         {
            _buffer[size] = '\0';
            printf("<< %s\n", _buffer); // ignore error
            if (strcmp("OK", _buffer) != 0)
            {
               fprintf(stderr, "<< Server error occured, abort\n");
               break;
            }
         }
      }
   } while (!isQuit);
}