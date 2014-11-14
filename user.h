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
		User(); //Default constructor
		User(string username, string password); //User-defined constructor
		~User();
		
		/* Setter */
		void setAccount(string username, string password);
		void addInbox(Message& msg);
		
		/* Getter */
		string getUsername();
		string getPassword();
		
		/* Other */
		void showInbox();
		
	private :
		string username;
		string password;
		vector<Message> inbox;
};

#endif
