/* 
 * File:   userServer.cpp
 * Author: dalva
 * 
 * Created on November 15, 2014, 2:59 PM
 */

#include "userServer.h"

userServer::userServer() {
}

userServer::userServer(const userServer& orig) {
	portNum = orig.portNum;
	isOnline = orig.isOnline;
}

userServer::~userServer() {
}

userServer::setPort(int port) {
	portNum = port;
}

userServer::setOnline() {
	isOnline = true;
}

userServer::setOffline() {
	isOnline = false;
}

userServer::getPort() {
	return portNum;
}

userServer::isOnline() {
	return isOnline;
}
