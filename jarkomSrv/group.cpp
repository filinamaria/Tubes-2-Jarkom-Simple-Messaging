#include "group.h"

Group::Group(){
	groupname = "";
}

Group::Group(string groupname){
	this->groupname = groupname;
}

Group::~Group(){
	
}

/* setter */
void Group::setGroupName(string groupname){
	this->groupname = groupname;
}

int Group::addMember(string username){
	vector<string>::iterator it;
	
	it = find(member.begin(), member.end(), username);
	if (it != member.end()){
		return 0;
	}else{
		member.push_back(username);
		Group::saveMember(username);
		return 1;
	}
}

void Group::addLog(string log, time_t timestamp){
	stringstream timestring;
	timestring << timestamp;
	string logstring = timestring.str() + ";" + log;
	this->log.push_back(logstring);
	Group::saveLog(logstring);
}

/* getter */
string Group::getGroupName(){
	return groupname;
}

vector<string> Group::getMemberList(){
	return member;
}

int Group::getMemberSize(){
	return member.size();
}

vector<string> Group::getLogList(){
	return log;
}

/* other methods */
int Group::delMember(string username){
	vector<string>::iterator it;
	
	it = find(member.begin(), member.end(), username);
	if (it != member.end()){
		int index = find(member.begin(), member.end(), username) - member.begin();
		member.erase(member.begin() + index);
		Group::deleteMemberFromExternalFile(username);
		return 1;
	}else{
		return 0;
	}
}

void Group::loadMemberFromExternalFile(){
	string member;
	string path = "Group/" + groupname + ".txt";
	ifstream myfile(path.c_str());
	
	while (myfile.is_open()) {
		while (getline(myfile, member)) {
			if(member != ""){
				this->member.push_back(member);
			}
		}
		myfile.close();
	}
}

void Group::loadLogFromExternalFile(){
	string log;
	string path = "Group-Log/" + groupname + ".txt";
	ifstream myfile(path.c_str());
	
	while (myfile.is_open()) {
		while (getline(myfile, log)) {
			if(log != ""){
				this->log.push_back(log);
			}
		}
		myfile.close();
	}
}

void Group::deleteMemberFromExternalFile(string username){
	vector<string> file;
	string temp;
	string path = "Group/" + groupname + ".txt";
	
	ifstream infile(path.c_str());

	while( !infile.eof() ){
		getline(infile, temp);
		file.push_back(temp);
	}
	
	infile.close();

	string item = username;

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

void Group::saveMember(string username){
	string path = "Group/" + groupname + ".txt";
	ofstream myfile(path.c_str(), ios_base::app);
	if (myfile.is_open()) {
		myfile << username << endl;
		myfile.close();
	}
}

void Group::saveLog(string log){
	string path = "Group-Log/" + groupname + ".txt";
	ofstream myfile(path.c_str(), ios_base::app);
	if (myfile.is_open()) {
		myfile << log << endl;
		myfile.close();
	}
}
