#ifndef LINDACOMMUNICATION_SOURCE_H
#define LINDACOMMUNICATION_SOURCE_H
#include <string>
#include <iostream>
#include <fstream>

class Source {
private:
	std::string command = "";
	unsigned int nr = 0;


public:

	Source() {}; 

	void nextLine() 
	{
		std::cout << "> ";
		command = "";
		nr = 0;
		getline(std::cin, command);
	}

	char nextChar() 
	{
		if (nr < command.length()) 
		{
			return command[nr++];
		}
		return -1;
	}
};

#endif