/* 
 * File:   userServer.h
 * Author: dalva
 *
 * Created on November 15, 2014, 2:59 PM
 */

#ifndef USERSERVER_H
#define	USERSERVER_H

#include "user.h"

class userServer : public User {
public:
	userServer();
	userServer(const userServer& orig);
	virtual ~userServer();
private:
	int portNum;
	bool isOnline;
	void setPort(int port);
	void setOnline();
	void setOffline();
	int getPort();
	bool isOnline();

};

#endif	/* USERSERVER_H */

