//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_SERVER_H
#define LINDACOMMUNICATION_SERVER_H

#include <string>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <memory>
#include <iostream>

#include "../shared/utils/confFile.h"

class Server
{
private:
    static const std::string sharedConfFilename;
    static const std::string serverConfFilename;

    int inputQueueId;
    int outputQueueId;
    int responseQueueId;
    int requestFileQueueId;
    int responseFileQueueId;

public:
    Server ();
    ~Server();

    void run ();

private:
    void init();
    int createMessageQueue (key_t key);
};

void * inputQueueThreadHandler (void * server);
void * outputQueueThreadHandler (void * server);

#endif //LINDACOMMUNICATION_SERVER_H
