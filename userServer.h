#ifndef USERSERVER_H
#define	USERSERVER_H

#include "message.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <vector>
#include <iterator>
#include <string>
#include <fstream>
using namespace std;

class UserServer{
	public:
		UserServer();
		UserServer(string username, string password);
		~UserServer();
		
		/* setter */
		void setUsername(string username);
		void setPassword(string password);
		void setStatus(bool status);
		void setPort(int portnumber);
		
		// menambah message ke dalam vector sekaligus external file
		void addPendingMessage(Message& message);
		
		/* getter */
		string getUsername();
		string getPassword();
		bool getStatus();
		int getPort();
		int getInboxSize();
		Message& getMessage(int index);
		
		/* other methods */	
		void loadMessages();
		void deleteMessage(int index);
		
	private:
		void saveMessage(Message& message);
		void deleteMessageFromExternalFile(string message);

	private:
		string username;
		string password;
		vector<Message> pendingmessages;
		bool status;
		int portnumber;
};

#endif	/* USERSERVER_H */

