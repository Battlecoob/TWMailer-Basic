// #include "include/command_class.hpp"

// bool Command::Send(void* socket) 
// {
//     int size;
//     char buffer[_buf];

//     bool inputReceived = false;

//     int *currentSocket = (int *) socket;
//     if (send(*currentSocket, "<Sender>", 7, 0) == -1)
//     {
//         std::cerr << "Send answer failed." << std::endl;
//         throw "Send answer failed.";
//     }
//     do
//     {
//         size = recv(*currentSocket, buffer, _buf - 1, 0);
//         if (size == -1)
//         {
//             if (_abortRequested)
//                 std::cerr << "recv error after aborted." << std::endl;

//             else
//                 std::cerr << "recv error." << std::endl;

//             break;
//         }

//         if (size == 0)
//         {
//             std::cout << "Client closed remote socket." << std::endl;
//             break;
//         }

//         if (buffer[size - 2] == '\r' && buffer[size - 1] == '\n')
//         {
//             size -= 2;
//         }
//         else if (buffer[size - 1] == '\n')
//         {
//             --size;
//         }

//         buffer[size] = '\0';
//         std::cout << "Message received: " << buffer << std::endl;

//         if(secureInput(buffer))
        
//     } while (!inputReceived);
    
// }

// bool Command::List() 
// {
//     return false;
// }

// bool Command::Read() 
// {
//     return false;
// }

// bool Command::Delete() 
// {
//     return false;
// }