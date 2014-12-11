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
void User::showMessages(string sender) {
	int count = 0;
	for (int i=0; i<this->inbox.size(); i++) {
		if (inbox[i].getSender()==sender) {
			inbox[i].showMessage();
			count++;
		}
	}
	if (count==0) {
		cout << "no recorded message from " << sender << endl;
	}
}

void User::loadMessages() {
	/* local variables */
	string line;
	string path = "User/" + username + ".txt";
	ifstream myfile(path.c_str());
	Message temp("","","","");
	
	/* algorithm */
	while (myfile.is_open()) {
		while (getline(myfile, line)) {
			temp.toMessage(line);
			inbox.push_back(temp);
		}
		myfile.close();
	}
}

void User::saveMessages() {
	string path = "User/" + username + ".txt";
	ofstream myfile(path.c_str());
	if (myfile.is_open()) {
		for (int i=0; i < inbox.size(); i++) {
			myfile << inbox[i].toString() << endl;
		}
		myfile.close();
	}
}
