//
// Created by sylwia on 12/4/17.
//

#include <iostream>
#include "Client.h"
//#include "../shared/linda/tupleTemplate.h"

Client *client;

int main()
{
	client = new Client();

	client->run(); //czytaj i wykonuj polecenia 

	delete client;

	//TupleTemplate tt;

    // tuple myTuple = makeTuple("si", "lol", 1, "xD");
    // printTuple(&myTuple);
    // char buffer[60];
    // int elementsCount = serializeTuple(&myTuple, buffer, 60);
    // std::cout << "ELEMENTS COUNT: " << elementsCount << std::endl;
    // tuple deserializedTuple = deserializeTuple(buffer);
    // printTuple(&deserializedTuple);
    // freeTuple(&myTuple);
    // freeTuple(&deserializedTuple);


    return 0;
}

