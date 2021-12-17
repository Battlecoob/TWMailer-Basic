all: client server

client: ./src/client/myclient.cpp
	g++ -std=c++17 -Wall -Werror -o ./bin/client ./src/client/myclient.cpp ./src/client/include/client_class.hpp ./src/client/client_class.cpp ./src/share/functions.cpp ./src/share/functions.hpp

server: ./src/server/myserver.cpp
	g++ -std=c++17 -Wall -Werror -o ./bin/server ./src/server/message_class.cpp ./src/server/myserver.cpp ./src/server/server_class.cpp ./src/server/include/message_class.hpp ./src/server/include/server_class.hpp ./src/server/include/database_class.hpp ./src/server/database_class.cpp ./src/server/user_class.cpp ./src/server/include/user_class.hpp ./src/share/functions.cpp ./src/share/functions.hpp -lstdc++fs

clean:
	rm -f ./bin/server ./bin/client