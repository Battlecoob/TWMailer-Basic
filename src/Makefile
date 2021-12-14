#############################################################################################
# Makefile
#############################################################################################
# G++ is part of GCC (GNU compiler collection) and is a compiler best suited for C++
CC=g++

# Compiler Flags: https://linux.die.net/man/1/g++
#############################################################################################
# -g: produces debugging information (for gdb)
# -Wall: enables all the warnings
# -Wextra: further warnings
# -O: Optimizer turned on
# -std: c++ 17 is the latest stable version c++2a is the upcoming version
# -I: Add the directory dir to the list of directories to be searched for header files.
# -c: says not to run the linker
# -pthread: Add support for multithreading using the POSIX threads library. This option sets 
#           flags for both the preprocessor and linker. It does not affect the thread safety 
#           of object code produced by the compiler or that of libraries supplied with it. 
#           These are HP-UX specific flags.
#############################################################################################
CFLAGS=-g -Wall -Wextra -O -std=c++2a -I /usr/local/include/gtest/ -pthread
GTEST=/usr/local/lib/libgtest.a

rebuild: clean all
all:  client server

clean:
	clear
	rm -f ./client ./server obj/*

myclient.o: myclient.cpp ./include/client_class.hpp
	$(CC) $(CFLAGS) -c myclient.cpp

client_class.o: ./include/client_class.hpp

client: myclient.o client_class.o
	$(CC) $(CFLAGS) -o ./client myclient.o client_class.o

myserver.o: myserver.cpp ./include/server_class.hpp ./include/command_class.hpp ./include/message_class.hpp ./include/send_class.hpp
	$(CC) $(CFLAGS) -c myserver.cpp

server_class.o: ./include/server_class.hpp

command_class.o: ./include/command_class.hpp ./include/message_class.hpp

message_class.o: ./include/message_class.hpp

send_class.o: ./include/send_class.hpp ./include/command_class.hpp

server: myserver.o server_class.o command_class.o message_class.o send_class.o
	$(CC) $(CFLAGS) -o ./server myserver.o server_class.o command_class.o message_class.o send_class.o