#ifndef LINDACOMMUNICATION_PARSE_H
#define LINDACOMMUNICATION_PARSE_H

#include "Scan.h"
#include "../shared/linda/tuple.h"
#include "../shared/linda/tupleTemplate.h"


typedef struct TupleOrTemplate // Union? struct? *?
{
	TupleTemplate tt;
	tuple t;
} TupleOrTemplate;

class Command 
{
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

	Atom accept(Atom at);
	Atom relOp();
	std::string acceptConst(Atom typ);

	void nextToken();

	Operator operatorFromAtom(Atom op);
public:
	Parse();
	~Parse();

	Command parseCommand();
	tuple parseTuple();


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
						tuplTemp.texts[tuplTemp.textNb].setValues(Any, elemNum, "*");
						tuplTemp.textNb++;
						break;
					}
					if ((op = relOp()) != null) {
						if (curToken.atom != stringConst)
							throw "Po operatorze porównania nie znaleziono stałej typu string";
						tuplTemp.texts[tuplTemp.textNb].setValues(operatorFromAtom(op), elemNum, curToken.value);
						nextToken();
						tuplTemp.textNb++;
						break;
					}
					if (curToken.atom == stringConst) {
						tuplTemp.texts[tuplTemp.textNb].setValues(Equals, elemNum, curToken.value);
						nextToken();
						tuplTemp.textNb++;
						break;
					}
					throw "Nie znaleziono ograniczeń dla pozycji";
				case integer:
					if (accept(starOp) != null) {
						tuplTemp.numbers[tuplTemp.numberNb].setValues(Any, elemNum, 0);
						tuplTemp.numberNb++;
						break;
					}
					if ((op = relOp()) != null) {
						if (curToken.atom != intConst)
							throw "Po operatorze porównania nie znaleziono stałej typu integer";
						tuplTemp.numbers[tuplTemp.numberNb].setValues(operatorFromAtom(op), elemNum, std::stoi(curToken.value));
						nextToken();
						tuplTemp.numberNb++;
						break;
					}
					if (curToken.atom == intConst) {
						tuplTemp.numbers[tuplTemp.numberNb].setValues(Equals, elemNum, std::stoi(curToken.value));
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
