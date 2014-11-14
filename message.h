#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <string>
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
		void setCurrentTimestamp();
		
		/* Getter */
		time_t getTimestamp();
		string getReadableTimestamp();
		string getSender();
		string getReceiver();
		string getText();
		
	private	:
		string sender;
		string receiver;
		string text;
		time_t timestamp;
};

#endif
