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

using namespace std;

class Server {
public:
	Server(int portnumber); //main constructs using this
	~Server();
	
	// Initialize, load user list. //main init using this, no loop.
	void init();
	
	void error(const char *msg);
	
	void initGarbageCollector();

private:
	//vector<Group> listofgroup;
	vector<SrvInstance> threads;
	thread garbageCollector;

	/*typedef struct {
		string eventmessage;
		time_t time;
	} event;
	vector<event> log;*/
	/*socklen_t clilen;
	char buffer[1024];
	struct sockaddr_in server_address, cli_addr;
	
	string messageBuf;*/

};

#endif
