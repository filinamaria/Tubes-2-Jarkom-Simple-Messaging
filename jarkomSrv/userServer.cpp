#include "userServer.h"

UserServer::UserServer(){
	username = "";
	password = "";
}

UserServer::UserServer(string username, string password){
	this->username = username;
	this->password = password;
}

UserServer::~UserServer(){
	
}

/* setter */
void UserServer::setUsername(string username){
	this->username = username;
}

void UserServer::setPassword(string password){
	this->password = password;
}

void UserServer::addPendingMessage(Message& message){
	pendingmessages.push_back(message);
	UserServer::saveMessage(message);
}

/* getter */
string UserServer::getUsername(){
	return username;
}

string UserServer::getPassword(){
	return password;
}

int UserServer::getInboxSize(){
	return pendingmessages.size();
}

Message& UserServer::getMessage(int index){
	return pendingmessages[index];
}

int UserServer::getMessageSize(){
	return pendingmessages.size();
}

/* other methods */
void UserServer::saveMessage(Message& message){
	string path = "User/" + username + "_" + password + ".txt";
	ofstream myfile(path.c_str(), ios_base::app);
	if (myfile.is_open()) {
		myfile << message.toString() << endl;
		myfile.close();
	}
}

void UserServer::addUserFile(const string& username, const string& password){
	string message;
	string path = "User/" + username + "_" + password + ".txt";
	ifstream myfile(path.c_str(), fstream::out | fstream::app);
	addUserToList(username, password);
}

void UserServer::loadMessages(){
	string message;
	string path = "User/" + username + "_" + password + ".txt";
	ifstream myfile(path.c_str());
	Message temp("","","","");
	
	while (myfile.is_open()) {
		while (getline(myfile, message)) {
			if(message != ""){
				temp.toMessage(message);
				pendingmessages.push_back(temp);
			}
		}
		myfile.close();
	}
	deleteAllMessages();
}

void UserServer::deleteMessage(int index){
	string message = pendingmessages[index].toString();
	UserServer::deleteMessageFromExternalFile(message);
	pendingmessages.erase(pendingmessages.begin() + index);
}

void UserServer::deleteAllMessages() {
	ifstream file;
	string path = "User/" + username + "_" + password + ".txt";
	file.open(path.c_str(), fstream::out | fstream::trunc);
	file.close();
	pendingmessages.erase(pendingmessages.begin(), pendingmessages.end());
}

void UserServer::deleteMessageFromExternalFile(string message){
	vector<string> file;
	string temp;
	string path = "User/" + username + "_" + password + ".txt";
	
	ifstream infile(path.c_str());

	while( !infile.eof() ){
		getline(infile, temp);
		file.push_back(temp);
	}
	
	infile.close();

	string item = message;

	for(int i = 0; i < (int)file.size(); ++i){
		if(file[i].substr(0, item.length()) == item){		
			file.erase(file.begin() + i);
			i = 0;
		}
	}

	ofstream out(path.c_str(), ios::out | ios::trunc);

	for(vector<string>::const_iterator i = file.begin(); i != file.end(); ++i){
		out << *i << endl;
	}
	out.close();
}

void UserServer::addUserToList(const string& username, const string& pass ){
	fstream userlist("User/list.txt", fstream::in | fstream::out | fstream::app);
	userlist << username << "_" << pass << endl;
}