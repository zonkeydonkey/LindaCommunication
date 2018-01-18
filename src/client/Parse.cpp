#include "Parse.h"

	void Parse::nextToken() 
	{
		curToken = scan->nextToken();
	}

	Parse::Parse() 
	{
		scan = new Scan();
	}

	Parse::~Parse() 
	{
		delete scan;
	}

	Atom Parse::accept(Atom at) 
	{
		if (curToken.atom == at) 
		{
			nextToken();
			return at;
		}
		return null;
	}

	Atom Parse::relOp() 
	{
		if (curToken.atom == greaterThan ||
			curToken.atom == greaterEqual ||
			curToken.atom == lessThan ||
			curToken.atom == lessEqual ||
			//curToken.atom == notEqual ||
			curToken.atom == equals)
		{
			Atom at = curToken.atom;
			nextToken();
			return at;
		}
		return null;
	}

	std::string Parse::acceptConst(Atom typ) 
	{
		Token val = curToken;
		switch (typ) 
		{
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
		nextToken();
		return val.value;
	}

	Operator Parse::operatorFromAtom(Atom op) 
	{
		switch (op) 
		{
		case greaterThan:
			return Greater;
		case greaterEqual:
			return GreaterOrEquals;
		case lessThan:
			return Less;
		case lessEqual:
			return LessOrEquals;
		case equals:
			return Equals;
		case starOp:
			return Any;
		default:
			throw "Nieoczekiwany operator";
		}
	}

	Command Parse::parseCommand() 
	{
		scan->nextLine();
		nextToken();
		Atom at = null;
		if ((at = accept(output)) == null)
			if (((at = accept(input)) == null))
				if ((at = accept(readInstr)) == null)
			throw "Dopuszczalne polecenia: input, output, read";
		if (accept(lBracket) == null) 
			throw "Oczekwano znaku (";

		time_t timeout = 0;
		TupleOrTemplate tUnion;
		switch (at) 
		{
			case output:
				tUnion.t = parseTuple();
				break;
			case input:
			case readInstr:
				tUnion.tt = parseTupleTemplate();
				timeout = parseTimeout();
				break;
			default:
			 	break;
		}
		if (accept(rBracket) == null) {
			throw "Oczekwano znaku )";
		}
		return Command(at, tUnion, timeout);
	}

	tuple Parse::parseTuple() 
	{
		std::string types = "";
		std::vector<std::string> values;
		while (true) 
		{
			Atom typ = null;
			if (((typ = accept(string)) == null) &&
				((typ = accept(integer)) == null)) {
				throw "Dopuszczalne typy elementów krotki - string, integer";
			}
			switch (typ) {
				case string: 
					types += STRING_TYPE_CHAR;
					break;
				case integer:
					types += INT_TYPE_CHAR;
					break;
				default: 
					break;
			}
			if (accept(colonOp) == null) {
				throw "Nie znaleziono znaku :";
			}
			values.push_back(acceptConst(typ)); 
			if (accept(commaOp) == null) {
				break;
			}
		}
		return makeTuple(values, types);
	}