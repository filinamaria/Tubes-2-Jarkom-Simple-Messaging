#include "message.h"
#include <iostream>

using namespace std;

int main(){
	Message newmessage("Mario", "Cilvia");
	newmessage.setText("hallo...");
	
	cout << "[" << newmessage.getReadableTimestamp() << "] " << newmessage.getSender() << " : " << newmessage.getText() << endl;
	return 0;
}
