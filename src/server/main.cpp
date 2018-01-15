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
    /*tuple myTuple = makeTuple("siii", "lol", 2, 3 ,4);
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
