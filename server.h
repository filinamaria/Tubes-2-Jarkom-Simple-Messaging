#ifndef _SERVER_H
#define _SERVER_H

#include "group.h"
#include "user.h"
#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <vector>

using namespace std;

class Server {
public:
	Server(int portnumber);
	~Server();

	// Start server,open port, load user list
	void startServer(int portnumber);

	//loop this
	void receiveMessage();

	// send message
	void sendMessageToClient();


	void error(const char *msg);

private:
	int portnumber;
	vector<Group> listofgroup;
	vector<User> listofuser;

	typedef struct {
		string eventmessage;
		time_t time;
	} event;
	vector<event> log;
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[1024];
	struct sockaddr_in server_address, cli_addr;

};

#endif
