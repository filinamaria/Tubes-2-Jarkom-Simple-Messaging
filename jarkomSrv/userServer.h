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
		
		// menambah message ke dalam vector sekaligus external file
		void addPendingMessage(Message& message);
		
		/* getter */
		string getUsername();
		string getPassword();
		int getInboxSize();
		Message& getMessage(int index);
		int getMessageSize();
		
		/* other methods */	
		void addUserFile();
		void loadMessages();
		void deleteMessage(int index); //seems unnecessary
		void deleteAllMessages();
		
		static void addUserToList(const string& username, const string& pass );
		
	private:
		void saveMessage(Message& message);
		void deleteMessageFromExternalFile(string message); //seems unnecessary
		
	private:
		string username;
		string password;
		vector<Message> pendingmessages;
};

#endif	/* USERSERVER_H */

