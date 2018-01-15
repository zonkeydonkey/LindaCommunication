//
// Created by sylwia on 12/4/17.
//

#include <iostream>
#include <signal.h>

#include "server.h"

using namespace std;

Server * server;

void interruptHandler(int signum)
{
    // na razie
    server->running = 0;
}

int main()
{
    server = new Server ();
    if(server->init() == -1)
        return -1;
    signal(SIGINT, interruptHandler);
    server->run();
    delete server;

    return 0;
}
