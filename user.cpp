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
void User::addMessageToInbox(Message& msg) {
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
void User::showMessages(string sender){
	int inboxsize = this->inbox.size();
	for (int i = 0; i < inboxsize; i++){
		if(inbox[i].getSender() == sender){
			cout << inbox[i].showReadableTimestamp() << " " << inbox[i].getSender() << " : " << inbox[i].getText() << endl;
		}
	}
}

void User::loadMessages(){
	
}

void User::saveMessages(){
	string Path("/User/" + username + ".txt");
	ifstream userfile (Path);
	if(userfile.is_open()){
		while(userfile.good()){
			
		}
		
	}
}
		
