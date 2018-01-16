#ifndef LINDACOMMUNICATION_CLIENT_H
#define LINDACOMMUNICATION_CLIENT_H
#include <string>
#include <iostream>
#include "Parse.h"


class Client {
	Parse *parse;

public:

	Client() 
	{
		parse = new Parse();
	}

	~Client() 
	{
		delete parse;
	}

	// void outputMessage(std::string toSave); 

	// void inputMessage(std::string templ, int itmeout);

	// void readMessage(std::string templ, int itmeout);

	void run() 
	{
		bool cmd;
		while (true)
		{
			cmd = parse->parseCommand();
		}
	}
};

#endif