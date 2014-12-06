/* 
 * File:   mainServer.cpp
 * Author: dalva
 *
 * Created on November 27, 2014, 7:26 PM
 */

#ifndef _SERVER_H
#define _SERVER_H

#include "srvInstance.h"
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
#include <chrono> 
#include <ctime>
#include <mutex>

using namespace std;

class SrvInstance;

class Server {
public:
	Server(int portnumber); //init server
	~Server();
	
	static void writeLog(const string& message);
	// add message functions
	static void addMessage(Message msg);
	static void addUserToList(const string& username, const string& pass );
	// Group Handler Functions
	static void deleteGroup();
	static void addUserToGroup(const string& groupname,const string& username);
	static void removeUserFromGroup(const string& groupname, const string username);
	
private:
	
	// Utility Functions
	void initGarbageCollector();
	static void error(const char *msg);
	static string formatCurTime();
	static bool fileExists(const string& name);
	static string getSubstr(const string& str, int start, char stop);

private:
	vector<SrvInstance> threads;
	//thread garbageCollector;
	static mutex logfileMutex;
	static fstream logfile;
	static mutex grouplistMutex;
	static mutex addMessageMutex;
	static fstream grouplist;
	static vector<Group> groups;
	static fstream userlist;
	static mutex userlistMutex;

	/* //not sure what this is...
	typedef struct {
		string eventmessage;
		time_t time;
	} event;
	 */
};

#endif
