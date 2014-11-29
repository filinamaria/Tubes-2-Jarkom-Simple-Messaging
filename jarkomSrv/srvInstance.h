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

#include <thread>
#include <mutex>

#define buflength 1024

using namespace std;

class SrvInstance {
public:
	SrvInstance(const SrvInstance&);
	
	//init new server
	SrvInstance(int sockid);
	
	virtual ~SrvInstance();
	
	//Threaded function to be called ONCE from serverThread
	void initThread();
	
	// send message
	void sendMessageToClient();
	
	//event handlers
	void signup();
	void login();
	void logout();
	void rcvMessage();
	void createGroup();
	void joinGroup();
	void leaveGroup();
	
	
	//utility functions
	string getSubstr(const string& str, int start, char stop);
	string getSubstrInt(const string& str, int start, int stop);
	
	void error(const char *msg);
	bool isDead();
	
	int getSockID();
	
private:
	UserServer usr;
	string messageBuf;
	int sockID;
	thread thrd;
	
	int sockErrCode;
	bool killFlag;

};

#endif	/* SERVERTHREAD_H */

