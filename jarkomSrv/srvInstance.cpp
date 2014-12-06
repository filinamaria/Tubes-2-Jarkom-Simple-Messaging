/* 
 * File:   SrvInstance.cpp
 * Author: dalva
 * 
 * Created on November 27, 2014, 9:03 PM
 */

#include "srvInstance.h"

SrvInstance::SrvInstance(const SrvInstance& in) {
	cout << "SrvInstance Class Copied to new handler" << endl;
	this->sockID = in.sockID;
	this->messageBuf = in.messageBuf;
	sockErrCode = 0;
	killFlag = false;
	connected = false;
}

SrvInstance::SrvInstance(int sockid) {
	cout << "SrvInstance Class Constructed" << endl;
	this->sockID = sockid;
	thrd = thread(&SrvInstance::initThread, this);
	messageBuf = "";
	sockErrCode = 0;
	killFlag = false;
	connected = false;
}

SrvInstance::~SrvInstance() {
	thrd.join();
	cout << "Object thread for connection " << sockID << " nuked." << endl;
}

void SrvInstance::initThread() {
	char buffer[buflength];
	int n;
	connected = true;
	
	cout << "Spawned and ran new thread for sockID: " << sockID << endl;
	
	while (connected) {
		//read a message
		bzero(buffer, buflength);
		n = read(sockID, buffer, buflength); /*** == IMPORTANT == ***/
		if (n < 0)
			error("ERROR reading from socket");
		int head = atoi(getSubstr(buffer, 0, ';').c_str());

		//process message accordingly
		switch (head) {
			case 1: // signup
				cout << "sockID " << sockID << " tries to sign up: " << buffer << endl;
				signup(getSubstrAdv(buffer, 2, ';'),getSubstrAdv(buffer, 3, ';'));
			return;
			case 2: // login
				cout << "sockID " << sockID << " tries to log in: " << buffer << endl;
				login(getSubstrAdv(buffer, 2, ';'),getSubstrAdv(buffer, 3, ';'));
			return;
			case 3: // logout
				cout << "sockID " << sockID << " tries to log out: " << buffer << endl;
				logout();
			return;
			case 4: // send message
				cout << "sockID " << sockID << " tries to send message: " << buffer << endl;
				rcvMessage(Message(	getSubstr(buffer, 1, ';'),
									getSubstr(buffer, 2, ';'),
									getSubstr(buffer, 4, ';'),
									getSubstr(buffer, 5, ';')));
			return;
			case 5: // create group
				cout << "sockID " << sockID << " tries to create a group: " << buffer << endl;
				createGroup(getSubstr(buffer, 1, ';'));
			return;
			case 6: // join group
				cout << "sockID " << sockID << " tries to join group: " << buffer << endl;
				joinGroup(getSubstr(buffer, 1, ';'));
			return;
			case 7: // leave group
				cout << "sockID " << sockID << " tries to leave group: " << buffer << endl;
				leaveGroup(getSubstr(buffer, 1, ';'));
			return;
			case 8: // show messages
				cout << "sockID " << sockID << " tries to show messages: " << buffer << endl;
				sendAllPending();
			return;
			default:
				cout << "client sent an invalid command: " << buffer << endl;
				messageBuf = "invalid"; // TODO: add to protocol
				return;
		}
		
		cout << "DBG: check for sock status" << endl;
		//check for socket status
		socklen_t len = sizeof(sockErrCode);
		if (getsockopt (sockID, SOL_SOCKET, SO_ERROR, &sockErrCode, &len) != 0) {
			cout << "sockID " << sockID << " forcefully disconnect."<< endl;
			killFlag = true;
			connected = false;
		}
	}

	//close current connection
	close(sockID);
	cout << "Connection " << sockID << " closed." << endl;
	terminate();
}

void SrvInstance::sendMessageToClient() {
	if (messageBuf != "") {
		int n;
		n = write(sockID,messageBuf.c_str(),messageBuf.length());
		if (n < 0) 
			 error("ERROR writing to socket");
		messageBuf = "";
	}
}

void SrvInstance::sendAllPending() {
	usr.loadMessages();
	for (int i = 0; i < usr.getMessageSize(); i++) {
		messageBuf = usr.getMessage(i).toString();
		sendMessageToClient();
	}
	messageBuf = "200;";
	sendMessageToClient();
	cout << "sendAllPending for user " << usr.getUsername() << endl;
}

/* begin main event handlers ---------------------------------------------------------- */

// PRIMARY TODO: finish this
void SrvInstance::signup(const string& usrName, const string& pass){
	if (fileExists("User/" + usrName + "_" + pass + ".txt")) {
		messageBuf = "1;fail;User already existed";
		sendMessageToClient();
	} else {
		usr.setUsername(usrName);
		usr.setPassword(pass);
		usr.addUserFile();
		messageBuf = "1;success";
		sendMessageToClient();
		sendAllPending();
		Server::writeLog(usrName + " logged in");
		cout << usrName << " logged in" << endl;
	}
}

void SrvInstance::login(const string& usrName, const string& pass){
	cout << "User/" + usrName + "_" + pass + ".txt" << endl;
	if (fileExists("User/" + usrName + "_" + pass + ".txt")) {
		usr.setUsername(usrName);
		usr.setPassword(pass);
		usr.loadMessages();
		messageBuf = "2;success";
		sendMessageToClient();
		sendAllPending();
		Server::writeLog(usrName + " logged in");
		cout << usrName << " logged in" << endl;
	} else {
		messageBuf = "2;fail;Invalid user name or password";
		Server::writeLog(usrName + " failed a log in attempt");
		cout << usrName << " failed a log in attempt" << endl;
		sendMessageToClient();
	}
}

void SrvInstance::logout(){
	connected = false;
	killFlag = true;
	messageBuf = "3;success";
	cout << usr.getUsername() << " logged out" << endl;
	sendMessageToClient();
}

void SrvInstance::rcvMessage(Message msg){
	msg.getReceiver();
	Server::addMessage(msg);
}

void SrvInstance::createGroup(const string& name){
	if (fileExists("Group/" + name + ".txt")) {
		messageBuf = "5;fail;Group already existed";
	} else {
		Server::addUserToGroup(name, usr.getUsername());
		messageBuf = "5;success";
		cout << usr.getUsername() << " created group " << << endl;
	}
	sendMessageToClient();
	sendAllPending();
}

void SrvInstance::joinGroup(const string& name){
	if (fileExists("Group/" + name + ".txt")) {
		Server::addUserToGroup(name, usr.getUsername());
		messageBuf = "6;success";
		cout << usr.getUsername() << " joined group " << name << endl;
	} else {
		messageBuf = "6;fail;Group is nonexistent";
	}
	sendMessageToClient();
	sendAllPending();
}

void SrvInstance::leaveGroup(const string& name){
	
	sendMessageToClient();
	sendAllPending();
}

void SrvInstance::sendMessageTo(const vector<string>& users, Message msg){
	for(int i=0; i<users.size(); i++) {
		//Server::
	}
}

/* begin utility handlers ---------------------------------------------------------- */

void SrvInstance::error(const char *msg) {
    perror(msg);
    exit(0);
}

string SrvInstance::getSubstr(const string& str, int start, char stop) {
	string temp="";
	int i;
	for (i=start; str[i]!=stop; i++) {
		temp = temp + str[i];
		if (temp.length() == str.length()-start) {
			break;
		}
	}
	start = i;
	return temp;
}
string SrvInstance::getSubstrAdv(const string& str, int start, char delimiter) {
	string temp="";
	int i;
	int len = 0;
	for (i=0; i<start; i++) {
		temp = getSubstr(str, len, delimiter);
		len = len + 1 + temp.length();
		
	}
	start = i;
	return temp;
}

string SrvInstance::getSubstrInt(const string& str, int start, int stop) {
	string temp="";
	int i;
	for (i=start; i<stop; i++)
		temp = temp + str[i];
	start = i;
	return temp;
}

bool SrvInstance::isDead() {
	return killFlag;
}

int SrvInstance::getSockID() {
	return sockID;
}

bool SrvInstance::fileExists(const string& name) {
    ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}