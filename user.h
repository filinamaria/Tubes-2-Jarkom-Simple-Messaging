#ifndef USER_H
#define USER_H

#include "message.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <vector>
#include <iterator>
#include <string>
using namespace std;

class User {
	public :
		User();
		User(string username, string password); //constructor
		~User();
		
		/* Setter */
		void addMessageToInbox(Message& msg);
		void setAccount(string username, string password);
		
		/* Getter */
		string getUsername();
		string getPassword();
		
		/* Other */
		void showMessages(string sender);
		void loadMessages();
		void saveMessages();
				
	private :
		string username;
		string password;
		vector<Message> inbox;
};

#endif
