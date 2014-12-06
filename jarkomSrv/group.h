#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

class Group {
	public:
		Group();
		Group(string groupname);
		~Group();
		
		/* setter */
		void setGroupName(string groupname);
		
		/* method addMember: return 0 jika gagal menambah user (user sudah ada), return 1 jika berhasil.
		 * 					 Menambah member ke vector sekaligus external file
		 */				 
		int addMember(string username);
		
		/* method addLog: Menambah log ke vector sekaligus external file */ 
		void addLog(string log, time_t timestamp);
		
		/* getter */
		string getGroupName();
		vector<string> getMemberList();
		vector<string> getLogList();
		int getMemberSize();
		
		/* other methods */
		/* method delMember: return 0 jika gagal (user tidak ada), return 1 jika berhasil
		 *			         Delete member dari vector dan external file
		 */
		int delMember(string username);
		void loadMemberFromExternalFile();
		void loadLogFromExternalFile();
		
	private: 
		void deleteMemberFromExternalFile(string username);
		void saveMember(string username);
		void saveLog(string log);
		
	private:
		string groupname;
		vector<string> member;
		vector<string> log;
};

#endif
