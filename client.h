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
#include <sstream>
using namespace std;

#define bufferSize 1024

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
		bool connectToHost();
		void loadUserData();
		void saveUserData();
		void error(const char *msg);
		void sendMessageToHost();
		string getMessageFromHost();
		void commandMenu();
		void processReply();
		void processReplyUntilStop();
	
	private :
		void signup();
		void login();
		void logout();
		void message();
		void createGroup();
		void joinGroup();
		void leaveGroup();
		void showMessages();
		bool isUserLogged();
		string getSubstr(const string& str, int start, char stop);
		string getSubstrInt(const string& str, int start, int stop);
		
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
