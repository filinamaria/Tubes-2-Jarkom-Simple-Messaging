#include "user.h"

User::User() {
	username = "";
	password = "";
}

User::User(string username, string password) { //constructor
	this->username = username;
	this->password = password;
}

User::~User() {}

/* Setter */
void User::addInbox(Message& msg) {
	inbox.push_back(msg);
}

void User::setAccount(string username, string password) {
	this->username = username;
	this->password = password;
}

/* Getter */
string User::getUsername() {
	return this->username;
}

string User::getPassword() {
	return this->password;
}

/* Other */
void User::showInbox() {
	for (int i=0; i<this->inbox.size(); i++) {
		time_t time = inbox[i].getTimestamp();
		tm *ltm = localtime(&time);
		cout << "[" << ltm->tm_year+1900 << "-" << ltm->tm_mon << "-" << ltm->tm_mday << " ";
		cout << ltm->tm_hour << ":" << ltm->tm_min << "] ";
		cout << inbox[i].getSender() << " : " << inbox[i].getText() << endl;
	}
}
		
