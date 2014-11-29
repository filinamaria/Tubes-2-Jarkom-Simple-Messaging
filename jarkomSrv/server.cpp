/* 
 * File:   mainServer.cpp
 * Author: dalva
 *
 * Created on November 27, 2014, 7:26 PM
 */

#include "server.h"

Server::Server(int portnumber) {
	int sockfd, newconn;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	
	//open socket, use AF_INET and SOCK_STEAM (TCP/IP) as protocol
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	cout << "socket opened..." << endl;

	//setup serv_addr, BIND TO PORT
	bzero((char *) &serv_addr, sizeof (serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnumber); /*** == IMPORTANT == ***/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
		error("ERROR on binding");
	cout << "port bind success..." << endl;
	
	//start garbage collector // LOW PRIORITY. UNFINIHSED.
	/*garbageCollector = thread(&Server::initGarbageCollector, this);
	cout << "garbage collector started..." << endl;*/
	
	//start listening
	listen(sockfd, 5);
	clilen = sizeof (cli_addr);
	cout << "we're online! " << "now listening." << endl;
	
	while (true) {
		//accept a connection
		newconn = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); /*** == IMPORTANT == ***/
		if (newconn < 0)
			error("ERROR on accept");
		
		cout << "New connection, sockID " << newconn << endl;
		//spawn a new thread to handle client connection
		//SrvInstance srvInst = SrvInstance((int) newconn);
		threads.push_back(SrvInstance(newconn)); /*** == IMPORTANT == ***/
		
	}
	close(sockfd);
}

Server::~Server() {
	garbageCollector.join();
}

void Server::error(const char *msg) {
    perror(msg);
    exit(0);
}

void Server::initGarbageCollector(){
	// LOW PRIORITY. UNFINIHSED.
	/*while (true) {
		std::this_thread::sleep_for (std::chrono::seconds(5));
		for(int i=0; i < threads.size(); i++) {
			if (threads[i].isDead()) {
				int sID = threads[i].getSockID();
				threads.erase(threads.begin() + i);
				cout << "[GarColle] erased allocated memory for SockID " << sID << endl;
			}
		}
	}*/
}