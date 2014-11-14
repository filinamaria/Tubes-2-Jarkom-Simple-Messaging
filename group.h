#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <string>
#include <vector>
#include <iterator>
#include "message.cpp"
using namespace std;

class Group {
	public :
		Group(string name); //constructor
		~Group();
		
		void join(string name);
		void sendMessage(Message msg); //dr client ada mekanisme utk menyimpan message yang sudah sampai
		void savePendingMessage(Message& msg);
		void leave(string name);
		void createEvent();
		void showLog();
		
		/* I/O */
		void loadData(string filename);
		void updateData(string filename);
	private :
		typedef struct {
			string username;
			time_t joinTime;
		} Member;		
		
		typedef struct {
			time_t timestamp;
			string news;
		} Event;

	private :
		string groupname;
		vector<Member> members;
		vector<Message> messages; //act as pending message in server-side, act as received message in client-side
		vector<Event> events;
};

#endif
