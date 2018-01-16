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
	Command() {}
};



class Parse {
private:
	Scan *scan;
	Token curToken;

	Atom accept(Atom at) 
	{
		if (curToken.atom == at) 
		{
			nextToken();
			return at;
		}
		return null;
	}

	void nextToken() {
		curToken = scan->nextToken();
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

		TupleOrTemplate tUnion;
		switch (at) 
		{
			case output:
				tUnion.t = parseTuple();
				break;
			case input:
			case read:
				tUnion.t = parseTuple();
			default:
			 	break;
		}
		if (accept(rBracket) == null) {
			throw "Oczekwano znaku )";
		}
		return Command(at, tUnion);
	}


	tuple parseTuple() 
	{
		std::string types = "";
		std::vector<std::string> values;
		unsigned elemNum = 0;
		while (true) 
		{
			Atom typ = null;
			if (((typ = accept(string)) == null) &&
				((typ = accept(integer)) == null)) {
				if (elemNum == 0 && curToken.atom == rBracket)
					return makeTuple("");
				throw "Dopuszczalne typy elementów krotki - string, integer";
			}
			switch (typ) {
				case string: 
					types += "s";
					break;
				case integer:
					types += "i";
					break;
				default: 
					break;
			}
			if (accept(colonOp) == null) {
				throw "Nie znaleziono znaku :";
			}

			Token val = curToken;
			switch (typ) {
				case string: 
					if (val.atom != stringConst)
						throw "Nie znaleziono stałej typu string";
					break;
				case integer:
					if (val.atom != intConst)
						throw "Nie znaleziono stałej typu integer";
					break;
				default: 
					break;
			}
			values.push_back(val.value);
			nextToken();
			elemNum++;
			if (accept(commaOp) == null) {
				break;
			}
		}
		return makeTuple("");
	}
};

#endif
