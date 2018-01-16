#ifndef LINDACOMMUNICATION_PARSE_H
#define LINDACOMMUNICATION_PARSE_H

#include "Scan.h"
#include "../shared/linda/tuple.h"
#include "../shared/linda/tupleTemplate.h"


typedef struct TupleOrTemplate // Union? struct? *?
{
	TupleTemplate tt;
	tuple t;
	TupleOrTemplate (TupleTemplate tt_): tt(tt_) {}
	TupleOrTemplate (tuple t_): t(t_) {}
	TupleOrTemplate () {}
} TupleOrTemplate;

class Command {
public:
	Atom cmd;
	TupleOrTemplate tupleUnion;
	Command(Atom a, TupleOrTemplate t): cmd(a), tupleUnion(t) {}
	Command(Atom a): cmd(a) {}
};



class Parse {
private:
	Scan *scan;
	Token curToken;

	Atom accept(Atom at) 
	{
		std::cout << curToken.atom  << "accept\n";
		if (curToken.atom == at) 
		{
			nextToken();
			return at;
		}
		return null;
	}

	void nextToken() {
		curToken = scan->nextToken();
		std::cout << "nextToken " << curToken.atom << std::endl;
	}


public:
	Parse() 
	{
		scan = new Scan();
	}

	~Parse() 
	{
		delete scan;
	}


	Command parseCommand() 
	{
		scan->nextLine();
		nextToken();
		Atom at = null;
		if ((at = accept(output)) == null)
			if (((at = accept(input)) == null))
				if ((at = accept(read)) == null)
			throw "Dopuszczalne polecenia: input, output, read";
		if (accept(lBracket) == null) 
			throw "Oczekwano znaku (";

		// TupleOrTemplate tUnion;
		// switch (at) 
		// {
		// 	case output:
		// 		// try {
		// 		// 	tUnion.t = parseTuple();
		// 		// } catch (const char* msg) {
		// 		// 	std::cout << msg << std::endl;
		// 		// 	return parseError();
		// 		// }
		// 		break;
		// 	case input:
		// 	case read:
		// 	default:
		// 	 	break;
		// }
		if (accept(rBracket) == null) {
			throw "Oczekwano znaku )";
		}
		return Command(at);
	}
};

#endif
