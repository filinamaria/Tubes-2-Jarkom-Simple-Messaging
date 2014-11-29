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
}

SrvInstance::SrvInstance(int sockid) {
	cout << "SrvInstance Class Constructed" << endl;
	this->sockID = sockid;
	thrd = thread(&SrvInstance::initThread, this);
	messageBuf = "";
	sockErrCode = 0;
	killFlag = false;
}

SrvInstance::~SrvInstance() {
	thrd.join();
	cout << "Thread for connection " << sockID << " nuked." << endl;
}

void SrvInstance::initThread() {
	char buffer[buflength];
	int n;
	bool connected = true;
	
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
			return;
			case 2: // login
				cout << "sockID " << sockID << " tries to log in: " << buffer << endl;
			return;
			case 3: // logout
				cout << "sockID " << sockID << " tries to log out: " << buffer << endl;
			return;
			case 4: // send message
				cout << "sockID " << sockID << " tries to send message: " << buffer << endl;
			return;
			case 5: // create group
				cout << "sockID " << sockID << " tries to create a group: " << buffer << endl;
			return;
			case 6: // join group
				cout << "sockID " << sockID << " tries to join group: " << buffer << endl;
			return;
			case 7: // leave group
				cout << "sockID " << sockID << " tries to leave group: " << buffer << endl;
			return;
			case 8: // show messages
				cout << "sockID " << sockID << " tries to show messages: " << buffer << endl;
			return;
			default:
				cout << "client sent an invalid command: " << buffer << endl;
				messageBuf = "invalid"; // TODO: add to protocol
				return;
		}
		
		//check for socket status
		socklen_t len = sizeof(sockErrCode);
		if (getsockopt (sockID, SOL_SOCKET, SO_ERROR, &sockErrCode, &len) != 0) {
			cout << "sockID " << sockID << " forcefully disconnect. Nuking thread."<< endl;
			killFlag = true;
			terminate();
		}
		
		//send reply
		sendMessageToClient();
		
		
	}

	//close current connection
	close(sockID);
	cout << "Connection " << sockID << " closed. Thread destroyed." << endl;
}

void SrvInstance::sendMessageToClient() {
	int n;
	
    n = write(sockID,messageBuf.c_str(),messageBuf.length());
    if (n < 0) 
         error("ERROR writing to socket");
}

/* begin main event handlers ---------------------------------------------------------- */
void SrvInstance::signup(){
	
}

void SrvInstance::login(){
	
}

void SrvInstance::logout(){
	
}

void SrvInstance::rcvMessage(){
	
}

void SrvInstance::createGroup(){
	
}

void SrvInstance::joinGroup(){
	
}

void SrvInstance::leaveGroup(){
	
}

/* begin utility handlers ---------------------------------------------------------- */

void SrvInstance::error(const char *msg) {
    perror(msg);
    exit(0);
}

string SrvInstance::getSubstr(const string& str, int start, char stop) {
	string temp="";
	int i;
	for (i=start; str[i]!=stop; i++) 
		temp = temp + str[i];
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