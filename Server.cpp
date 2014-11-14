#include "Server.h"

Server::Server(int portnumber){
	this->portnumber = portnumber;
	Server::startServer(portnumber);
}

Server::~Server(){
	close(newsockfd);
	close(sockfd);
}

void Server::startServer(int portnumber){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &server_address, sizeof(server_address));
	portno = portnumber;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &server_address,
		  sizeof(server_address)) < 0) 
		  error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(client_address);
	newsockfd = accept(sockfd, 
			 (struct sockaddr *) &client_address, 
			 &clilen);
	if (newsockfd < 0) 
	  error("ERROR on accept");
}

void Server::error(const char *msg){
	
}
