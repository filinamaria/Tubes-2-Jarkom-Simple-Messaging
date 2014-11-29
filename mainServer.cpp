/* 
 * File:   mainServer.cpp
 * Author: dalva
 *
 * Created on November 28, 2014, 2:49 PM
 */

#include <iostream>
#include "server.h"
#include "group.h"
#include "userServer.h"
#include "message.h"

using namespace std;

/*
 * Server Main
 */
int main(int argc, char** argv) {

	if (argc < 1) {
		cout << endl << " ==== WMD MESSENGER -- SERVER ==== " << endl;
		cout << "usage: ./server <port>" << endl << endl;
		return 1;
	}

	cout << endl << " ==== WMD MESSENGER -- SERVER ==== " << endl;
	cout << "Starting Server..." << endl << endl;
	int portno = atoi(argv[1]);

	Server serverHandler(portno);
	serverHandler.init();
	return 0;
}

