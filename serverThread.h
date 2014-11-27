/* 
 * File:   serverThread.h
 * Author: dalva
 *
 * Created on November 27, 2014, 7:58 PM
 */

#ifndef SERVERTHREAD_H
#define	SERVERTHREAD_H

#include "group.h"
#include "user.h"
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

class serverThread {
public:
	serverThread();
	serverThread(const serverThread& orig);
	virtual ~serverThread();
	serverThread(short port);
private:
	typedef struct {
		string eventmessage;
		time_t time;
	} event;
	int portnumber;
	//vector<Group> listofgroup;
	vector<User> listofuser;
	vector<event> log;
};

#endif	/* SERVERTHREAD_H */

