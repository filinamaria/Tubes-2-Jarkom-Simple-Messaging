#include "server.h"

Server::Server(int portnumber) {
	this->portnumber = portnumber;
	startServer(portnumber);
}

Server::~Server() {
}

void Server::startServer(int portnumber) {
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &server_address, sizeof (server_address));
	portno = portnumber;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &server_address, sizeof (server_address)) < 0)
		error("ERROR on binding");
	listen(sockfd, 5);
	clilen = sizeof (cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	bzero(buffer, 1024);
	n = read(newsockfd, buffer, 255);
	if (n < 0) error("ERROR reading from socket");
	printf("Here is the message: %s\n", buffer);
	n = write(newsockfd, "I got your message", 18);
	if (n < 0) error("ERROR writing to socket");
	close(newsockfd);
	close(sockfd);
}

void Server::error(const char *msg) {

}
