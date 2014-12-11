#include "message.h"

Message::Message(string sender, string receiver, string text, string type) { //constructor
	this->sender = sender;
	this->receiver = receiver;
	this->text = text;
	this->timestamp = time(0);
	this->type = type;
}

Message::Message(const Message& m) { //copy constructor
	this->sender = m.sender;
	this->receiver = m.receiver;
	this->text = m.text;
	this->timestamp = m.timestamp;
	this->type = m.type;
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

void Message::setType(string type) {
	this->type = type;
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

string Message::getType() {
	return this->type;
}

/* Other */
string Message::toString() {
	stringstream ss;
	ss << timestamp;
	return "4;" + sender + ";" + receiver + ";" + ss.str() + ";" + text + ";" + type;
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
		temp = getSubstr(msg, i, ';');
		setSender(temp);
		temp = "";
		//receiver
		i++;
		temp = getSubstr(msg, i, ';');
		setReceiver(temp);
		temp = "";
		//timestamp
		i++;
		temp = getSubstr(msg, i, ';');
		istringstream buffer(temp);
		int x;
		buffer >> x;
		t_temp = x;
		setTimestamp(t_temp);
		temp = "";
		//message body
		i++;
		temp = getSubstr(msg, i, ';');
		text = temp;
		temp = "";
		//type
		i++;
		temp = getSubstrInt(msg, i, len);
		type = temp;
	}
	//else do nothing
}

string Message::getSubstr(const string& str, int& start, char stop) {
	string temp="";
	int i;
	for (i=start; str[i]!=stop; i++) 
		temp = temp + str[i];
	start = i;
	return temp;
}

string Message::getSubstrInt(const string& str, int& start, int stop) {
	string temp="";
	int i;
	for (i=start; i<stop; i++)
		temp = temp + str[i];
	start = i;
	return temp;
}

void Message::showReadableTimestamp() {
	time_t time = timestamp;
	tm *ltm = localtime(&time);
	cout << "[" << ltm->tm_year+1900 << "-" << ltm->tm_mon << "-" << ltm->tm_mday << " ";
	cout << ltm->tm_hour << ":" << ltm->tm_min << "] ";
}

void Message::showMessage() {
	showReadableTimestamp();
	cout << sender << " : " << text << endl;
}

