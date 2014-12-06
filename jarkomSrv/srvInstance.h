/* 
 * File:   serverThread.h
 * Author: dalva
 *
 * Created on November 27, 2014, 9:03 PM
 */

#ifndef SERVERTHREAD_H
#define	SERVERTHREAD_H

#include "group.h"
#include "userServer.h"
#include "message.h"
#include "server.h"
#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <vector>
#include <ctime>

#include <thread>
#include <mutex>

#define buflength 1024

using namespace std;

class Server;

class SrvInstance {
public:
	SrvInstance(const SrvInstance&);
	SrvInstance(int sockid); //init new server
	virtual ~SrvInstance();
private:
	//Thread init function to be called ONCE
	void initThread();
	
	// send message
	void sendMessageToClient();
	
	// send all pending messages. do this after every command that requires 200 retcode.
	void sendAllPending();
	
	//getter
	bool isDead();
	int getSockID();
	
	//event handlers
	void signup(const string& usrName, const string& pass); //DONE
	void login(const string& usrName, const string& pass); //DONE
	void logout(); //DUN
	void rcvMessage(Message msg);
	void createGroup(const string& name);
	void joinGroup(const string& name);
	void leaveGroup(const string& name);
	
	//sendMessage function
	void sendMessageTo(const vector<string>& users, Message msg);
	
	//utility functions
	string getSubstr(const string& str, int start, char stop);
	string getSubstrAdv(const string& str, int start, char delimiter);
	string getSubstrInt(const string& str, int start, int stop);
	void error(const char *msg);
	static bool fileExists(const string& name);
	
private: //vars
	UserServer usr;
	string messageBuf;
	int sockID;
	thread thrd;
	
	int sockErrCode;
	bool killFlag;
	bool connected;

};

#endif	/* SERVERTHREAD_H */

