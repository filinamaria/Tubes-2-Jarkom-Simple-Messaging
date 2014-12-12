
CC=g++

instruction : 
	@echo usage:
	@echo make client
	@echo make server
	@echo make clean

server : main.cpp server.cpp srvInstance.cpp userServer.cpp message.cpp group.cpp
	g++ -o server main.cpp server.cpp srvInstance.cpp userServer.cpp message.cpp group.cpp -std=c++11

client : mainClient.cpp client.cpp message.cpp user.cpp
	g++ -o client mainClient.cpp client.cpp message.cpp user.cpp

server.o : server.cpp
	g++ -c server.cpp

clean :
	rm -f server client *.o

.PHONY: clean
.PHONY: instruction
