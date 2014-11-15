compile : mainClient.cpp user.o client.o message.o
	g++ -o client mainClient.cpp usero client.o message.o

user.o : user.cpp user.h
	g++ -o user.cpp

client.o : client.c client.h
	g++ -o client.c

message.o : message.cpp message.h
	g++ -o message.cpp
