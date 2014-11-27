

CC=g++
CFLAGS=-I

instruction : 
	@echo usage:
	@echo make client
	@echo make server
	@echo make clean

server : mainServer.cpp server.cpp userServer.cpp message.cpp group.cpp
	g++ -o server mainServer.cpp server.cpp userServer.cpp message.cpp group.cpp

client : mainClient.cpp client.cpp message.cpp user.cpp
	g++ -o client mainClient.cpp client.cpp message.cpp user.cpp

server.o : server.cpp
	g++ -c server.cpp

clean :
	rm -f server client

.PHONY: clean
.PHONY: instruction