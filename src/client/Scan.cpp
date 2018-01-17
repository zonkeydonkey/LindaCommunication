#include "Scan.h"
#include <utility>

	Keywords::Keywords() {
		keywords.push_back(std::make_pair("output",output));
		keywords.push_back(std::make_pair("input",input));
		keywords.push_back(std::make_pair("read",readInstr));
		keywords.push_back(std::make_pair("integer",integer));
		keywords.push_back(std::make_pair("string",string));
	}

	Atom Keywords::find(std::string str) 
	{
		for (size_t i = 0; i != keywords.size(); ++i)
			if(keywords[i].first == str)
				return keywords[i].second;
		return null;
	}


	Scan::Scan()
	{
		src = new Source();
		kwords = new Keywords();
	}

	Scan::~Scan()
	{
		delete src;
		delete kwords;
	}

	void Scan::nextLine() 
	{
		src->nextLine();
		curChar = src->nextChar();
	}


	Token Scan::scanKword() 
	{
		std::string str = ""; 
		while ((curChar != -1) && isLetter(curChar)) {
			str += curChar;
			curChar = src->nextChar();
		}
		return Token(kwords->find(str));
	}

	Token Scan::scanNumber(std::string scannedPart) 
	{
		if (curChar == '0') {
			curChar = src->nextChar();
			if (isDigit(curChar))
				throw "Wiele zer wiodących";
			return Token(intConst, "0");
		}
		while ((curChar != -1) && isDigit(curChar))
		{
			scannedPart += curChar;
			curChar = src->nextChar();
		}
		return Token(intConst, scannedPart);
	}

	Token Scan::scanStringConst(char qMark) // na "
	{
		std::string str = ""; 
		curChar = src->nextChar();
		while (curChar != qMark)
		{
			if (curChar == -1) 
				throw "Niezakończony string";
			str += curChar;
			curChar = src->nextChar();
		}
		curChar = src->nextChar();
		return Token(stringConst, str);
	}


	bool Scan::isLetter(char i) 
	{
		if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z'))
			return true;
		else
			return false;
	}

	bool Scan::isWhitespace(int cChar) {
		if ((cChar > 0 && cChar < 33) || cChar == 127)
			return true;
		return false;
	}

	bool Scan::isDigit(int cChar) {
		if ((cChar >= '0' && cChar <= '9'))
			return true;
		return false;
	}


	Token Scan::nextToken() 
	{
		do {
			while (isWhitespace(curChar)) 
				curChar = src->nextChar();
			if (curChar == -1) // EOF
				return Token(endl);
		} while (isWhitespace(curChar));
		if (isLetter(curChar))
			return scanKword();
		if (isDigit(curChar))
			return scanNumber(std::string(""));
		switch(curChar) {
		case '-':
			curChar = src->nextChar();
			if (isDigit(curChar))
				return scanNumber(std::string("-"));	
			else
				throw "Brak stałej liczbowej po operatorze -";
		case '"':
			return scanStringConst(curChar);
		case '(': 
			curChar = src->nextChar();
			return Token(lBracket);
		case ')': 
			curChar = src->nextChar();
			return Token(rBracket);
		case ':': 
			curChar = src->nextChar();
			return Token(colonOp);
		case ',': 
			curChar = src->nextChar();
			return Token(commaOp);
		case '>':
			curChar = src->nextChar();
			if (curChar == '=') {
				curChar = src->nextChar();
				return Token(greaterEqual);
			}
			return Token(greaterThan);
		case '<':
			curChar = src->nextChar();
			if (curChar == '=') {
				curChar = src->nextChar();
				return Token(lessEqual);
			}
			return Token(lessThan);
		case '=':
			curChar = src->nextChar();
			if (curChar == '=') {
				curChar = src->nextChar();
				return Token(equals);
			}
			throw "Po znaku = oczekiwano =";
		// case '!':
		// 	curChar = src->nextChar();
		// 	if (curChar == '=') {
		// 		curChar = src->nextChar();
		// 		return Token(notEqual);
		// 	}
		// 	throw "Po znaku ! oczekiwano =";
		case '*':
			curChar = src->nextChar();
			return Token(starOp);
		}
		return Token(null);
	}