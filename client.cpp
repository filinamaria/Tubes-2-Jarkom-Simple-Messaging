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
bool Client::connectToHost() {
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
        return false;
	}
	else return true;
}

void Client::loadUserData() {
	activeUser.loadMessages();
}

void Client::saveUserData() {
	activeUser.saveMessages();
}

void Client::error(const char *msg) {
    perror(msg);
    exit(0);
}

void Client::sendMessageToHost() {
	/* local variables */
	char buffer[bufferSize];
	int n;
	
	/* algorithm */
    n = write(sockfd,messageToHost.c_str(),messageToHost.length());
    if (n < 0) 
         error("ERROR writing to socket");
}

string Client::getMessageFromHost() {
	/* local variables */
	char buffer[bufferSize];
	int n;
	
	/* algorithm */
	bzero(buffer,bufferSize);
    n = read(sockfd,buffer,bufferSize);
    if (n < 0) 
         error("ERROR reading from socket");
    string temp(buffer);
    return temp;
}

void Client::commandMenu() {
	/* local variables */
	string command;
	bool ok = false, waitUntilStop = true;
	
	/* algorithm */
	printf("> ");
	cin >> command;
	if (command=="signup") {
		signup(); 
		ok = true;
		waitUntilStop = false;
	}
	else if (command=="login") {
		if (!isUserLogged()) {
			login(); 
			ok = true;
		}
		else printf("Someone already logged in!");
	}
	else if (isUserLogged()) {
		if (command=="logout") {
			logout();
			ok = true;
			waitUntilStop = false;
		}
		else if (command=="message") {
			message(); 
			ok = true;
		}
		else if (command=="create") {
			createGroup();
			ok = true;
		}
		else if (command=="join") {
			joinGroup();
			ok = true;
		}
		else if (command=="leave") {
			leaveGroup();
			ok = true;
		}
		else if (command=="show") {
			showMessages();
			ok = false;
		}
		else printf("wrong command!\n");
	}
	else {
		printf("wrong command!\n");
	}
	
	if (ok) {
		sendMessageToHost();
		if (!waitUntilStop)
			processReply();
		else {
			processReplyUntilStop();
		}
	}
}

void Client::processReply() {
	/* local variables */
	string temp, argv1, argv2;
	string head;
	bool stop = false;
	
	/* algorithm */
	do { 
		temp = getMessageFromHost();
		head = getSubstr(temp, 0, ';');
		
		
		if (head=="1") {
			argv1 = getSubstr(temp, 2, ';');
			if (argv1=="fail") {
				argv2 = getSubstrInt(temp, 7, temp.length());
				cout << argv2 << endl;
			}
			else { //success
				stop = true;
			}
		}
		else if (head=="3") {
			argv1 = getSubstr(temp, 2, ';');
			stop = true;
			if (argv1=="success")
				stop = true;
		}
	} while (!stop);
}

void Client::processReplyUntilStop() {
	/* local variables */
	string temp, argv1, argv2;
	string head;
	bool stop = false, written = false;
	
	/* algorithm */
	temp = getMessageFromHost();
	head = getSubstr(temp, 0, ';');
	while (head!="200") {
		argv1 = getSubstr(temp, 2, ';');
		if (head=="2" || head=="5" || head=="6" || head=="7") {
			if (argv1=="fail") {
				argv2 = getSubstrInt(temp, 7, temp.length());
				cout << endl << argv2 << endl;
			}
			if (head=="2" && argv1=="success") {
				loadUserData();
			}
		}
		else if (head=="4") {
			if (argv1=="success") {
				printf("Message Sent!");
			}
			else { //ini berarti ada message baru
				if (!written) printf("New Message(s)\n"); 
				Message msg("","","","");
				msg.toMessage(temp);
				activeUser.addInbox(msg);
				activeUser.saveMessages();
				msg.showMessage();
				written = true;
			}
		}
		messageToHost = "200;";
		sendMessageToHost();
		temp = getMessageFromHost();
		head = getSubstr(temp, 0, ';');
	}
}

void Client::signup() {
	/* local variables */
	string argvMessage1, argvMessage2; 
	
	/* algorithm */
	printf("username : "); cin >> argvMessage1;
	printf("password : "); cin >> argvMessage2;
	messageToHost = "1;" + argvMessage1 + ";" + argvMessage2;
}

void Client::login() {
	/* local variables */
	string argvMessage1, argvMessage2; 
	
	/* algorithm */
	printf("username : "); cin >> argvMessage1; getchar();
	printf("password : "); cin >> argvMessage2;
	activeUser.setAccount(argvMessage1, argvMessage2);
	messageToHost = "2;" + argvMessage1 + ";" + argvMessage2;
}

void Client::logout() {
	/* local variables */
	string argvMessage1, argvMessage2; 
	
	/* algorithm */
	printf("username : "); cin >> argvMessage1;
	saveUserData();
	activeUser.setAccount(" ", " ");
	messageToHost = "3;" + argvMessage1;
}

void Client::message() {
	/* local variables */
	string argvMessage1, argvMessage2; 
	
	/* algorithm */
	cin >> argvMessage1; //receiver
	getchar();
	printf("Message : \n");
	getline(cin,argvMessage2); //text
	Message newMsg(activeUser.getUsername(), argvMessage1, argvMessage2, "personal");
	messageToHost = newMsg.toString();
}

void Client::createGroup() {
	/* local variables */
	string argvMessage1;
	
	/* algorithm */
	cin >> argvMessage1;
	messageToHost = "5;" + argvMessage1;
}

void Client::joinGroup() {
	/* local variables */
	string argvMessage1;
	
	/* algorithm */
	cin >> argvMessage1;
	messageToHost = "6;" + argvMessage1;
}

void Client::leaveGroup() {
	/* local variables */
	string argvMessage1;
	
	/* algorithm */
	cin >> argvMessage1;
	messageToHost = "7;" + argvMessage1;
}

void Client::showMessages() {
	/* local variables */
	string argvMessage1;
	
	/* algorithm */
	cin >> argvMessage1;
	activeUser.showMessages(argvMessage1);
}

bool Client::isUserLogged() {
	return !(activeUser.getUsername()=="" || activeUser.getUsername()==" ");
}

string Client::getSubstr(const string& str, int start, char stop) {
	string temp="";
	int i;
	for (i=start; str[i]!=stop; i++) 
		temp = temp + str[i];
	start = i;
	return temp;
}

string Client::getSubstrInt(const string& str, int start, int stop) {
	string temp="";
	int i;
	for (i=start; i<stop; i++)
		temp = temp + str[i];
	start = i;
	return temp;
}
