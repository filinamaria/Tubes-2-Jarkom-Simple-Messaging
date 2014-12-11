#include "client.h"
#include "message.h"
#include "user.h"
#include <iostream>

int main(int argc, char *argv[]) {
	
	if(argc < 2) {
		cout << endl << " ==== WMD MESSENGER -- CLIENT ==== " << endl;
		cout << "usage: ./client <servername> <port>" << endl << endl;
		return 1;
	}
    
	cout << endl << " ==== WMD MESSENGER -- CLIENT ==== " << endl;
	cout << "Starting Client..." << endl << endl;
    string host(argv[1]);
    int portno = atoi(argv[2]);
    
    Client clientHandler(portno, host);
    clientHandler.connectToHost();
    //clientHandler.processReply(); //minta reply sampai port bener
    
    while (1) {
		clientHandler.commandMenu();
	}
    return 0;
}
