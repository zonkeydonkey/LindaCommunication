#ifndef LINDACOMMUNICATION_PARSE_H
#define LINDACOMMUNICATION_PARSE_H

#include "Scan.h"

class Parse {
private:
	Scan *scan;
	Atom curToken;

	bool accept(Atom at) 
	{
		if (curToken == at) 
		{
			curToken = scan->nextToken();
			return true;
		} 
		return false;
	}


public:
	Parse() {
		scan = new Scan();
	}

	~Parse() {
		delete scan;
	}

	bool parseCommand() {
		scan->nextLine();
		curToken = scan->nextToken();
		if (!accept(output) && !accept(input) && !accept(read)) {
			std::cout << "Błąd w poleceniu. Spróbuj ponownie\n";
			return false;
		}
		if (!accept(lBracket)) {
			std::cout << "Błąd w poleceniu. Spróbuj ponownie\n";
			return false;
		}
		while (true) 
		{
			if (!accept(string) && !accept(integer)) {
				std::cout << "Błąd w poleceniu. Spróbuj ponownie\n";
				return false;
			}
			if (!accept(colonOp)) {
				std::cout << "Błąd w poleceniu. Spróbuj ponownie\n";
				return false;
			}
			if (!accept(greaterThan) && !accept(greaterEqual) &&
				!accept(lessThan) && !accept(lessEqual) &&
				!accept(notEqual) && !accept(equals) &&
				!accept(intConst) && !accept(stringConst) && 
				!accept(starOp)) {
				std::cout << "Błąd w poleceniu. Spróbuj ponownie\n";
				return false;
			}
			if (!accept(commaOp)) {
				break;
			}
		}
		if (!accept(rBracket))
			std::cout << "Błąd w poleceniu. Spróbuj ponownie\n";
		return true;
	}
};

#endif