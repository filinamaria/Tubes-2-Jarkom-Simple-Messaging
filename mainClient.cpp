#include "client.h"
#include "message.h"
#include "user.h"

int main(int argc, char *argv[]) {
    char buffer[256];
    
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
