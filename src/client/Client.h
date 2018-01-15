

#ifndef LINDACOMMUNICATION_CLIENT_H
#define LINDACOMMUNICATION_CLIENT_H
#include <string>
#include <iostream>

class Client {

public:
	bool parseCommand() ;

	void outputMessage(std::string toSave); //zamiast stringa tuple

	void inputMessage(std::string toSave);
};

#endif