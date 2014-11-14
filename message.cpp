#include "message.h"

Message::Message(string sender, string receiver) { //constructor
	setSender(sender);
	setReceiver(receiver);
	setCurrentTimestamp();
}

Message::Message(const Message& m) { //copy constructor
	this->sender = m.sender;
	this->receiver = m.receiver;
	this->text = m.text;
	this->timestamp = m.timestamp;
}
		
Message::~Message() {}

/* Setter */
void Message::setSender(string sender) {
	this->sender = sender;
	
}

void Message::setReceiver(string receiver) {
	this->receiver = receiver;
}

void Message::setText(string text) {
	this->text = text;
}

void Message::setCurrentTimestamp() {
	time_t now = time(0);
	this->timestamp = now;
}

/* Getter */
time_t Message::getTimestamp() {
	return this->timestamp;
}

string Message::getReadableTimestamp(){
	tm * ptm = localtime(&timestamp);
	cout << &ptm << endl;
	char timestring[32];
	strftime(timestring, 32, "%Y-%m-%d %H:%M:%S", ptm);  
	string time(timestring);
	return time;
}

string Message::getSender() {
	return this->sender;
}

string Message::getReceiver() {
	return this->receiver;
}

string Message::getText() {
	return this->text;
}

