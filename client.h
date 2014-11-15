#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctime>
using namespace std;

class Client {
	public :
		Client(int portno, string hostname); //constructor
		~Client();
		
		/* Setter */
		void setPortno(int portno);
		void setHostname(string hostname);
		void setActiveUser(string username, string password);
		
		/* Getter */
		int getPortno();
		string getHostname();
		User getActiveUser();
		
		/* Other */
		void connectToHost();
		void loadUserData(string filename);
		void saveUserData(string filename);
		void error(const char *msg);
		void sendMessageToHost();
		void receiveMessageFromHost();
		void commandMenu();
		
	private :
		int portno;
		int sockfd;
		struct sockaddr_in serv_addr;
		struct hostent *server;
		string messageToHost;
		string hostname;
		User activeUser;
		bool all;
};

#endif
