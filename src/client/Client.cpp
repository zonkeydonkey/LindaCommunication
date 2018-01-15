
#include "Client.h"
using namespace std;

	bool Client::parseCommand() {
		string command;
		cin >> command;
		if (command == "a") {}
			else
			if (command == "q") 
				return false;
			else
				if (command == "q") 
				{
					cout << "Niepoprawne polecenie\n";
				}
		return true;
	}

	void Client::outputMessage(string toSave) {

	}


	void Client::inputMessage(string toSave) {

	}