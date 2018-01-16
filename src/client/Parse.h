#ifndef LINDACOMMUNICATION_PARSE_H
#define LINDACOMMUNICATION_PARSE_H

#include "Scan.h"
#include "../shared/linda/tuple.h"

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

	tuple parseError() {
		std::cout << "Błąd w poleceniu. Spróbuj ponownie\n";
		return makeTuple("");
	}

	tuple parseCommand() {
		std::string types = "";
		scan->nextLine();
		curToken = scan->nextToken();
		if (!accept(output) && !accept(input) && !accept(read)) {
			return parseError();
		}
		if (!accept(lBracket)) {
			return parseError();
		}
		
		while (true) 
		{
			if (!accept(string) && !accept(integer)) {
				return parseError();
			}
			if (!accept(colonOp)) {
				return parseError();
			}
			if (!accept(greaterThan) && !accept(greaterEqual) &&
				!accept(lessThan) && !accept(lessEqual) &&
				!accept(notEqual) && !accept(equals) &&
				!accept(intConst) && !accept(stringConst) && 
				!accept(starOp)) {
				return parseError();
			}
			if (!accept(commaOp)) {
				break;
			}
		}
		if (!accept(rBracket))
			std::cout << "Błąd w poleceniu. Spróbuj ponownie\n";
		return makeTuple("si", "lol", 1, "xD");
	}

	// tuple parseTupleTemplate() {

	// }
};

#endif