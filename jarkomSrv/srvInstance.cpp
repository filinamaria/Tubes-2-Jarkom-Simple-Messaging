/* 
 * File:   SrvInstance.cpp
 * Author: dalva
 * 
 * Created on November 27, 2014, 9:03 PM
 */

#include "srvInstance.h"

SrvInstance::SrvInstance(const SrvInstance& in) {
	//cout << "SrvInstance Class Copied to new handler" << endl;
	this->sockID = in.sockID;
	this->messageBuf = in.messageBuf;
	sockErrCode = 0;
	killFlag = false;
	connected = false;
}

SrvInstance::SrvInstance(int sockid) {
	cout << "SrvInstance Class Constructed" << endl;
	this->sockID = sockid;
	messageBuf = "";
	sockErrCode = 0;
	killFlag = false;
	connected = false;
}

void SrvInstance::start() {
	cout << "starting thread..." << endl;
	thrd = thread(&SrvInstance::initThread, this);
}

SrvInstance::~SrvInstance() {
    if(thrd.joinable()) thrd.join();
	//cout << "an bject thread for connection " << sockID << " nuked." << endl;
}

void SrvInstance::initThread() {
	char buffer[buflength];
	int n;
	connected = true;
	
	cout << "Spawned and ran new thread for sockID: " << sockID << endl;
	int invalids = 0;
	
	while (connected) {
		//read a message
		bzero(buffer, buflength);
		n = read(sockID, buffer, buflength); /*** == IMPORTANT == ***/
		if (n < 0) {
			killFlag = true;
		}
		int head = atoi(getSubstr(buffer, 0, ';').c_str());

		//process message accordingly
		switch (head) {
			case 1: // signup
				invalids = 0;
				cout << "sockID " << sockID << " tries to sign up: " << buffer << endl;
				signup(getSubstrAdv(buffer, 2, ';'),getSubstrAdv(buffer, 3, ';'));
			break;
			case 2: // login
				invalids = 0;
				cout << "sockID " << sockID << " tries to log in: " << buffer << endl;
				login(getSubstrAdv(buffer, 2, ';'),getSubstrAdv(buffer, 3, ';'));
			break;
			case 3: // logout
				invalids = 0;
				cout << "sockID " << sockID << " tries to log out: " << buffer << endl;
				logout();
			break;
			case 4: // send message
				invalids = 0;
				cout << "sockID " << sockID << " tries to send message: " << buffer << endl;
				rcvMessage(Message(	getSubstrAdv(buffer, 2, ';'),
									getSubstrAdv(buffer, 3, ';'),
									getSubstrAdv(buffer, 4, ';'),
									getSubstrAdv(buffer, 5, ';')));
			break;
			case 5: // create group
				invalids = 0;
				cout << "sockID " << sockID << " tries to create a group: " << buffer << endl;
				createGroup(getSubstrAdv(buffer, 2, ';'));
			break;
			case 6: // join group
				invalids = 0;
				cout << "sockID " << sockID << " tries to join group: " << buffer << endl;
				joinGroup(getSubstrAdv(buffer, 2, ';'));
			break;
			case 7: // leave group
				invalids = 0;
				cout << "sockID " << sockID << " tries to leave group: " << buffer << endl;
				leaveGroup(getSubstrAdv(buffer, 2, ';'));
			break;
			case 8: // show messages
				invalids = 0;
				cout << "sockID " << sockID << " tries to show messages: " << buffer << endl;
				// show message is supposed to be client-side right? sorting thru client side inbox
				// because server always send new message after each 200-capable command.
				//showMessage(getSubstrAdv(buffer, 2, ';'));
				sendAllPending();
			break;
			default:
				invalids++;
		}
		
		//check for socket status
		socklen_t len = sizeof(sockErrCode);
		if ((getsockopt (sockID, SOL_SOCKET, SO_ERROR, &sockErrCode, &len) != 0) || invalids > 10 || killFlag) {
			cout << "sockID " << sockID << " forcefully disconnect."<< endl;
			connected = false;
		}
	}

	//close current connection
	close(sockID);
	cout << "Connection " << sockID << " closed." << endl;
}

void SrvInstance::sendMessageToClient() {
	if (messageBuf != "") {
		int n;
		messageBuf = messageBuf + "\n";
		n = write(sockID,messageBuf.c_str(),buflength);
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
}

/* begin main event handlers ---------------------------------------------------------- */

// PRIMARY TODO: finish this
void SrvInstance::signup(const string& usrName, const string& pass){
	if (fileExists("User/" + usrName + "_" + pass + ".txt")) {
		messageBuf = "1;fail;User already existed";
		sendMessageToClient();
	} else {
		messageBuf = "1;success";
		sendMessageToClient();
		usr.addUserFile(usrName, pass);
		Server::writeLog(usrName + " signed up");
		cout << usrName << " signed up" << endl;
	}
}

void SrvInstance::login(const string& usrName, const string& pass){
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
	cout << usr.getUsername() << " logged out" << endl;
	Server::writeLog(usr.getUsername() + " logged out");
	messageBuf = "3;success;";
	sendMessageToClient();
	//connected = false;
}

void SrvInstance::rcvMessage(Message msg){
	Server::addMessage(msg);
	messageBuf = "4;success;";
	sendMessageToClient();
	sendAllPending();
}

void SrvInstance::createGroup(const string& name){
	if (fileExists("Group/" + name + ".txt")) {
		messageBuf = "5;fail;Group already existed";
	} else {
		Server::addUserToGroup(name, usr.getUsername());
		messageBuf = "5;success";
		cout << usr.getUsername() << " created group " << name << endl;
		Server::writeLog(usr.getUsername() + " created group " + name);
	}
	sendMessageToClient();
	sendAllPending();
}

void SrvInstance::joinGroup(const string& name){
	if (fileExists("Group/" + name + ".txt")) {
		Server::addUserToGroup(name, usr.getUsername());
		messageBuf = "6;success";
		cout << usr.getUsername() << " joined group " << name << endl;
		Server::writeLog(usr.getUsername() + " joined group " + name);
	} else {
		messageBuf = "6;fail;Group is nonexistent";
	}
	sendMessageToClient();
	sendAllPending();
}

void SrvInstance::leaveGroup(const string& name){
	if (Server::removeUserFromGroup(name, usr.getUsername())){
		messageBuf = "7;success";
		cout << usr.getUsername() << " leaved group " << name << endl;
		Server::writeLog(usr.getUsername() + " leaved group " + name);
	} else {
		messageBuf = "7;fail;User is not inside group, or Group is nonexistent";
	}
	sendMessageToClient();
	sendAllPending();
}

void SrvInstance::showMessage(const string& name){
				// show message is supposed to be client-side right? sorting thru client side inbox
				// because server always send new message after each 200-capable command.
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