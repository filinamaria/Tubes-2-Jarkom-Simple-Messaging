#include "client.h"

Client::Client(int portno, string hostname) { //constructor
	setPortno(portno);
	setHostname(hostname);
	all = false;
}

Client::~Client() {
	close(sockfd);
}

/* Setter */
void Client::setPortno(int portno) {
	this->portno = portno;
}

void Client::setHostname(string hostname) {
	this->hostname = hostname;
}

void Client::setActiveUser(string username, string password) {
	activeUser.setAccount(username,password);
}

/* Getter */
int Client::getPortno() {
	return portno;
}

string Client::getHostname() {
	return hostname;
}

User Client::getActiveUser() {
	return activeUser;
}

/* Other */
void Client::connectToHost() {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { 
        error("ERROR opening socket");
        exit(0);
	}
	cout << hostname << endl;
    server = gethostbyname(hostname.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
	}
}

void Client::loadUserData(string filename) {
}

void Client::saveUserData(string filename) {

}

void Client::error(const char *msg) {
    perror(msg);
    exit(0);
}

void Client::sendMessageToHost() {
	/* local variables */
	char buffer[1024];
	int n;
	
	/* algorithm */
    n = write(sockfd,messageToHost.c_str(),messageToHost.length());
    if (n < 0) 
         error("ERROR writing to socket");
}

void Client::receiveMessageFromHost() {
	/* local variables */
	char buffer[1024];
	int n;
	
	/* algorithm */
	bzero(buffer,1024);
    n = read(sockfd,buffer,1024);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
}

void Client::commandMenu() {
	/* local variables */
	string command, argvMessage1, argvMessage2; 
	
	/* algorithm */
	printf("> ");
	cin >> command;
	if (command=="signup") {
		printf("username : "); cin >> argvMessage1;
		printf("password : "); cin >> argvMessage2;
		messageToHost = "1;" + argvMessage1 + ";" + argvMessage2;
		sendMessageToHost();
		receiveMessageFromHost();
	}
	else if (command=="login") {
		printf("username : "); cin >> argvMessage1;
		printf("password : "); cin >> argvMessage2;
		messageToHost = "2;" + argvMessage1 + ";" + argvMessage2;
		sendMessageToHost();
		receiveMessageFromHost();
	}
	else if (command=="logout") {
		printf("username : "); cin >> argvMessage1;
		messageToHost = "3;" + argvMessage1;
		sendMessageToHost();
		receiveMessageFromHost();
	}
	else if (command=="message") {
		cin >> argvMessage1; //receiver
		getchar();
		printf("Message : \n");
		getline(cin,argvMessage2); //text
		Message newMsg(activeUser.getUsername(), argvMessage1);
		newMsg.setText(argvMessage2);
		messageToHost = newMsg.toString();
		sendMessageToHost();
		receiveMessageFromHost();
	}
	else {
		printf("Belum siap kaka\n");
	}
}
