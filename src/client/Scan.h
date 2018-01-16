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
	notEqual, 		//15
	equals,			//16
	starOp,			//17
	null,			//18
	endl			//19
};



class Keywords {
private:
	typedef std::vector<std::pair<std::string, Atom> > Map;
	Map keywords;
public:
	Keywords();
	
	Atom find(std::string str) 
	{
		for (size_t i = 0; i != keywords.size(); ++i)
			if(keywords[i].first == str)
				return keywords[i].second;
		return null;
	}
};



class Scan {
private: 
	Source *src;
	char curChar;
	Keywords *kwords;

	Atom scanKword() 
	{
		std::string str = ""; 
		while ((curChar != -1) && isLetter(curChar)) {
			str += curChar;
			curChar = src->nextChar();
		}
		return kwords->find(str);
	}

	Atom scanNumber() 
	{
		if (curChar == '0') {
			curChar = src->nextChar();
			if (isDigit(curChar))
				return null;
			return intConst;
		}
		int i = 0;
		while ((curChar != -1) && isDigit(curChar))
		{
			i = 10*i + curChar;
			curChar = src->nextChar();
		}
		return intConst;
	}

	Atom scanStringConst(char qMark) // na "
	{
		std::string str = ""; 
		curChar = src->nextChar();
		while (curChar != qMark)
		{
			if (curChar == -1) 
				return null;
			str += curChar;
			curChar = src->nextChar();
		}
		curChar = src->nextChar();
		return stringConst;
	}

	bool isLetter(char i) 
	{
		if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z'))
			return true;
		else
			return false;
	}

	bool isWhitespace(int cChar) {
		if ((cChar > 0 && cChar < 33) || cChar == 127)
			return true;
		return false;
	}

	bool isDigit(int cChar) {
		if ((cChar >= '0' && cChar <= '9'))
			return true;
		return false;
	}

public:
	Scan()
	{
		src = new Source();
		kwords = new Keywords();
	}

	~Scan()
	{
		delete src;
		delete kwords;
	}

	void nextLine() 
	{
		src->nextLine();
		curChar = src->nextChar();
	}

	Atom nextToken() 
	{
		do {
			while (isWhitespace(curChar)) 
				curChar = src->nextChar();
			if (curChar == -1) // EOF
				return endl;
		} while (isWhitespace(curChar));
		if (isLetter(curChar))
			return scanKword();
		if (isDigit(curChar))
			return scanNumber();
		switch(curChar) {
		case '"':
			return scanStringConst(curChar);
		case '(': 
			curChar = src->nextChar();
			return lBracket;
		case ')': 
			curChar = src->nextChar();
			return rBracket;
		case ':': 
			curChar = src->nextChar();
			return colonOp;
		case ',': 
			curChar = src->nextChar();
			return commaOp;
		case '>':
			curChar = src->nextChar();
			if (curChar == '=') {
				curChar = src->nextChar();
				return greaterEqual;
			}
			return greaterThan;
		case '<':
			curChar = src->nextChar();
			if (curChar == '=') {
				curChar = src->nextChar();
				return lessEqual;
			}
			return lessThan;
		case '=':
			curChar = src->nextChar();
			if (curChar == '=') {
				curChar = src->nextChar();
				return equals;
			}
			return null;
		case '!':
			curChar = src->nextChar();
			if (curChar == '=') {
				curChar = src->nextChar();
				return notEqual;
			}
			return null;
		case '*':
			curChar = src->nextChar();
			return starOp;
		}
		return null;
	}
};

#endif