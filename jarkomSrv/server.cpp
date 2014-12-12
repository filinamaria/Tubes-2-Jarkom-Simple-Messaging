/* 
 * File:   mainServer.cpp
 * Author: dalva
 *
 * Created on November 27, 2014, 7:26 PM
 */

#include "server.h"

// STATIC VAR REDEFINITIONS
fstream Server::logfile("log.txt", fstream::out | fstream::app);
mutex Server::logfileMutex;
fstream Server::grouplist("Group/list.txt", fstream::in | fstream::out | fstream::app);
mutex Server::grouplistMutex;
vector<Group> Server::groups;
fstream Server::userlist("User/list.txt", fstream::in | fstream::out | fstream::app);
mutex Server::addMessageMutex;
mutex Server::userlistMutex;

Server::Server(int portnumber) {
	
	int sockfd, newconn;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	
	//open socket, use AF_INET and SOCK_STEAM (TCP/IP) as protocol
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	cout << " socket opened..." << endl;

	//setup serv_addr, BIND TO PORT
	bzero((char *) &serv_addr, sizeof (serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnumber); /*** == IMPORTANT == ***/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
		error("ERROR on binding");
	cout << " port bind success..." << endl;
	
	//start garbage collector // LOW PRIORITY. UNFINIHSED.
	/*garbageCollector = thread(&Server::initGarbageCollector, this);
	cout << "garbage collector started..." << endl;*/
	
	//Load groups
	cout << " Loading groups..." << endl;
	string line;
	while (getline(grouplist,line)) {
		if (fileExists("Group/" + line + ".txt")) {
			cout << "  + group [" << line << "] members:" << endl;
			groups.push_back(Group(line));
			groups.back().loadMemberFromExternalFile();
			vector<string> membersDEBUG = groups.back().getMemberList();
			for (int i=0; i<membersDEBUG.size(); i++) {
				cout << "   - " << membersDEBUG[i] << endl;
			}
		}
	}
	
	//start listening
	listen(sockfd, 5);
	clilen = sizeof (cli_addr);
	cout << "We're online! now listening." << endl;
	
	writeLog("Server starts");
	
	//main connection acceptor loop
	while (true) {
		//accept a connection
		newconn = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); /*** == IMPORTANT == ***/
		if (newconn < 0)
			error("ERROR on accept");
		
		cout << "New connection, sockID " << newconn << endl;
		//spawn a new thread to handle client connection
		//SrvInstance srvInst = SrvInstance((int) newconn);
		threads.push_back(SrvInstance(newconn)); /*** == IMPORTANT == ***/
		cout << "constructed, Tlen=" << threads.size() << endl;
		threads.back().startT();
		
	}
	close(sockfd);
}

Server::~Server() {
	//garbageCollector.join();
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

string Server::formatCurTime(){
	string retval;
	time_t t = time(0);
    struct tm * now = localtime( & t );
	string year = to_string(now->tm_year + 1900);
	string month = to_string(now->tm_mon + 1);
	string date = to_string(now->tm_mday);
	string hour = to_string(now->tm_hour);
	string minute = to_string(now->tm_min);
	string second = to_string(now->tm_sec);
    retval = "[" + year + "-" + month + "-" + date + " " + hour + ":" + minute + ":" + second + "]";
	return retval;
}

void Server::writeLog(const string& message) {
	logfileMutex.lock();
	logfile << formatCurTime() << " " << message << endl;
	cout << formatCurTime() << " " << message << endl;
	logfileMutex.unlock();
}

bool Server::fileExists(const string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

void Server::addMessage(Message msg){
	userlist.seekg (0, userlist.beg);
	string rcvr = msg.getReceiver();
	string buf;
	string pass;
	string usr;
	addMessageMutex.lock();
	bool foundInUsers = false;
	bool foundInGroups = false;
	userlist.close();
	userlist.open("User/list.txt", fstream::in);
	while (getline(userlist,buf)) {
		usr = getSubstr(buf, 0, '_');
		pass = getSubstr(buf, 1, '_');
		if (usr.compare(rcvr) == 0) {
			fstream file;
			file.open("User/" + buf + ".txt", fstream::out | fstream::app);
			file << msg.toString() << endl;
			file.close();
			foundInUsers = true;
		}
	}
	if (!foundInUsers) {
		grouplist.close();
		grouplist.open("Group/list.txt", fstream::in | fstream::out | fstream::app);
		while (getline(grouplist,buf)) {
			if (buf.compare(rcvr) == 0) {
				foundInGroups = true;
				fstream file;
				file.open("Group/" + buf + ".txt", fstream::in);
				string groupmember;
				while (getline(file, groupmember)) {
					while (getline(userlist,buf)) {
						usr = getSubstr(buf, 0, '_');
						if (usr == groupmember) {
							fstream userfile;
							userfile.open("User/" + buf + ".txt", fstream::out | fstream::app);
							userfile << msg.toString() << endl;
							userfile.close();
						}
					}
				}
				msg.setType(buf);
				file << msg.toString() << endl;
				file.close();
			}
		}
	}
	addMessageMutex.unlock();
}

/*
 void Server::addMessage(Message msg){
	userlist.seekg (0, userlist.beg);
	string rcvr = msg.getReceiver();
	string buf;
	string pass;
	string usr;
	addMessageMutex.lock();
	getline(userlist,buf);
	usr = getSubstr(buf, 0, '_');
	pass = getSubstr(buf, 1, '_');
	while (usr.compare(rcvr) != 0) {
		getline(userlist,buf);
		usr = getSubstr(buf, 0, '_');
		pass = getSubstr(buf, 1, '_');
	}
	if (usr.compare(rcvr) == 0) {
		fstream file;
		file.open("User/" + buf + ".txt", fstream::out | fstream::app);
		file << msg.toString() << endl;
		file.close();
	} else {
		getline(grouplist,buf);
		while (buf.compare(rcvr) != 0) {
			getline(grouplist,buf);
		}
		if (buf.compare(rcvr) == 0) {
			fstream file;
			file.open("Group/" + buf + ".txt", fstream::in);
			string groupmember;
			while (getline(file, groupmember)) {
				while (getline(userlist,buf)) {
					usr = getSubstr(buf, 0, '_');
					pass = getSubstr(buf, 1, '_');
					if (buf == groupmember)
				}
			}
			msg.setType(buf);
			file << msg.toString() << endl;
			file.close();
		}
	}
	addMessageMutex.unlock();
}
 
 
 */

string Server::getSubstr(const string& str, int start, char stop) {
	string temp="";
	int i;
	for (i=start; str[i]!=stop; i++) 
		temp = temp + str[i];
	start = i;
	return temp;
}

void Server::addUserToList(const string& username, const string& pass ){
	userlistMutex.lock();
	userlist << username << "_" << pass << endl;
	userlistMutex.unlock();
}

void Server::addUserToGroup(const string& groupname,const string& username){
	bool found = false;
	grouplistMutex.lock();
	for (int i=0; i<groups.size(); i++) {
		if (groups[i].getGroupName() == groupname) {
			groups[i].addMember(username);
			found =true;
		}
	}
	if (!found) {
		//string path = "Group/" + groupname + ".txt";
		//fstream gfile(path.c_str(), fstream::out | fstream::app);
		//gfile << username << endl;
		grouplist.close();
		grouplist.open("Group/list.txt", fstream::out | fstream::app);
		grouplist << groupname << endl;
		grouplist.close();
		grouplist.open("Group/list.txt", fstream::in | fstream::out | fstream::app);
		groups.push_back(Group(groupname));
		groups.back().addMember(username);
	}
	grouplistMutex.unlock();
}

bool Server::removeUserFromGroup(const string& groupname, const string username){
	grouplistMutex.lock();
	for (int i=0; i<groups.size(); i++) {
		if (groups[i].getGroupName() == groupname) {
			groups[i].delMember(username);
			if (groups[i].getMemberSize() == 0) {
				groups.erase(groups.begin()+i);
			}
			grouplistMutex.unlock();
			return true;
		}
	}
	grouplistMutex.unlock();
	return false;
}