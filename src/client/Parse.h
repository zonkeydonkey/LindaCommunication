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

	Atom relOp() {
		if (curToken.atom == greaterThan ||
			curToken.atom == greaterEqual ||
			curToken.atom == lessThan ||
			curToken.atom == lessEqual ||
			curToken.atom == notEqual ||
			curToken.atom == equals)
		{
			Atom at = curToken.atom;
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
				tUnion.tt = parseTupleTemplate();
				break;
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

	TupleTemplate parseTupleTemplate() 
	{
		TupleTemplate tuplTemp;
		unsigned elemNum = 0;
		while (true) 
		{
			Atom typ = null;
			if (((typ = accept(string)) == null) &&
				((typ = accept(integer)) == null)) {
				if (elemNum == 0 && curToken.atom == rBracket)
					return tuplTemp;
				throw "Dopuszczalne typy elementów krotki - string, integer";
			}
			if (accept(colonOp) == null) {
				throw "Nie znaleziono znaku :";
			}

			Atom op;
			switch (typ) 
			{
				case string: 
					if (accept(starOp) != null) {
						tuplTemp.texts[tuplTemp.textNb].setValues(elemNum, "*");
						tuplTemp.textNb++;
						break;
					}
					if ((op = relOp()) != null) {
						if (curToken.atom != stringConst)
							throw "Po operatorze porównania nie znaleziono stałej typu string";
						// else znowu setvalues
						nextToken();
						tuplTemp.textNb++;
						break;
					}
					if (curToken.atom == stringConst) {
						//tuplTemp.texts[tuplTemp.textNb].setValues(elemNum, curToken.value);
						
						nextToken();
						tuplTemp.textNb++;
						break;
					}
					throw "Nie znaleziono ograniczeń dla pozycji";
				case integer:
					if ((op = relOp()) != null) {
						if (curToken.atom != intConst)
							throw "Po operatorze porównania nie znaleziono stałej typu integer";
						
						// iiiii setvalues
						nextToken();
						tuplTemp.numberNb++;
						break;
					}
					if (curToken.atom == intConst) {
						
						nextToken();
						tuplTemp.numberNb++;
						break;
					}						
					throw "Nie znaleziono ograniczeń dla pozycji";
				default:
					break;
			}

			elemNum++;
			if (accept(commaOp) == null) {
				break;
			}
		}
		return tuplTemp;
	}
};

#endif
