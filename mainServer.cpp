#include "Server.h"

using namespace std;

int main(int argc, char *argv[]){
	int port = atoi(argv[1]);
	
	Server serverhandler(port);
	return 0;
}
