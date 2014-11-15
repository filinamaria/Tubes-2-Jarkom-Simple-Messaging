#include "message.h"

Message::Message(string sender, string receiver) { //constructor
	setSender(sender);
	setReceiver(receiver);
	this->timestamp = time(0);
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

void Message::setTimestamp(time_t timestamp) {
	this->timestamp = timestamp;
}

/* Getter */
time_t Message::getTimestamp() {
	return this->timestamp;
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

/* Other */
string Message::toString() {
	stringstream ss;
	ss << timestamp;
	return "4;" + sender + ";" + receiver + ";" + ss.str() + ";" + text;
}

void Message::toMessage(string msg) {
	/* local variables */
	int i, len, x;
	string temp = "";
	time_t t_temp;
	
	/* algorithm */
	if (msg[0]=='4') {
		i = 2;
		len = msg.length();
		//sender
		while (msg[i]!=';') {
			temp = temp + msg[i];
			i++;
		}
		setSender(temp);
		temp = "";
		//receiver
		while (msg[i]!=';') {
			temp = temp + msg[i];
			i++;
		}
		setReceiver(temp);
		temp = "";
		//timestamp
		while (msg[i]!=';') {
			temp = temp + msg[i];
			i++;
		}
		istringstream buffer(temp);
		int x;
		buffer >> x;
		t_temp = time(0) + x;
		setTimestamp(t_temp);
		temp = "";
		//message body
		while (i<len) {
			temp = temp + msg[i];
			i++;
		}
		text = temp;
	}
	//else do nothing
}

