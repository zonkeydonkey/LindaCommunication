//
// Created by sylwia on 12/4/17.
//

#include <iostream>
#include <signal.h>
#include "server.h"
#include "../shared/linda/tuple.h"

Server * server;

void interruptHandler(int signum)
{
    server->stop();
}

int main()
{
    /*tuple myTuple = makeTuple("sis", "lol", 1, "xD");
    printTuple(&myTuple);
    char buffer[60];
    int elementsCount = serializeTuple(&myTuple, buffer, 60);
    std::cout << "ELEMENTS COUNT: " << elementsCount << std::endl;
    tuple deserializedTuple = deserializeTuple(buffer);
    printTuple(&deserializedTuple);
    freeTuple(&myTuple);
    freeTuple(&deserializedTuple);*/
    server = new Server ();
    if(server->init() == -1)
        return -1;
    signal(SIGINT, interruptHandler);
    server->run();
    delete server;

    return 0;
}
