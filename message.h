#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
using namespace std;

class Message {
	public	:
		Message(string sender, string receiver); //constructor
		Message(const Message& m); //copy constructor
		~Message();
		
		/* Setter */
		void setSender(string sender);
		void setReceiver(string receiver);
		void setText(string text);
		void setTimestamp(time_t timestamp);
		
		/* Getter */
		time_t getTimestamp();
		string getSender();
		string getReceiver();
		string getText();
		
		/* Other */
		string toString();
		void toMessage(string msg);
		void showReadableTimestamp();
		void showMessage();
		
	private	:
		string sender;
		string receiver;
		string text;
		time_t timestamp;
};

#endif
