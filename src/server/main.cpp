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
    // na razie
    server->stop();
}

int main()
{
    /*tuple myTuple = makeTuple("si", "lol", 2);
    printTuple(&myTuple);
    freeTuple(&myTuple);*/
    server = new Server ();
    if(server->init() == -1)
        return -1;
    signal(SIGINT, interruptHandler);
    server->run();
    delete server;

    return 0;
}
