#ifndef LINDACOMMUNICATION_SCAN_H
#define LINDACOMMUNICATION_SCAN_H

#include <string>
#include <iostream>
#include <vector>

#include "Source.h"

enum Atom {
	output,			//0
	input,			//1
	read,			//2
	integer,		//3
	string,			//4
	intConst,		//5
	stringConst,	//6
	lBracket,		//7
	rBracket,		//8
	colonOp,		//9
	commaOp,		//10
	greaterThan,	//11
	greaterEqual,	//12
	lessThan,		//13
	lessEqual,		//14
	equals,			//15
	starOp,			//16
	null,			//17
	endl			//18
	// notEqual
};


class Token
{
public:
	Atom atom;
	std::string value;
	Token(Atom at): atom(at) {};
	Token(Atom at, std::string val): atom(at), value(val) {};
	Token() {};
};


class Keywords {
private:
	typedef std::vector<std::pair<std::string, Atom> > Map;
	Map keywords;
public:
	Keywords();
	Atom find(std::string str);
};



class Scan {
private: 
	Source *src;
	char curChar;
	Keywords *kwords;

	Token scanKword();
	Token scanNumber(std::string scannedPart);
	Token scanStringConst(char qMark);

	bool isLetter(char i);
	bool isWhitespace(int cChar);
	bool isDigit(int cChar);

public:
	Scan();
	~Scan();
	void nextLine();
	
	Token nextToken();
};

#endif