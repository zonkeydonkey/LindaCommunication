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

#include "FileWorker.h"
#include "../shared/utils/confFile.h"
#include "../shared/messages/outputMessage.h"
#include "../shared/messages/shared.h"

class Server
{
protected:
    static const std::string sharedConfFilename;
    static const std::string serverConfFilename;
    static const std::string tupleSpaceConfFilename;

    int inputQueueId;
    int outputQueueId;
    int responseQueueId;
    int requestFileQueueId;
    int responseFileQueueId;
    std::string tupleSpaceFile;

    volatile bool running = true;

public:
    Server ();
    ~Server ();

    int init();
    void run ();
    void stop();

    friend void * inputQueueThreadHandler (void * server);
    friend void * outputQueueThreadHandler (void * server);
    friend void * fileWorkerThreadHandler (void * server);

private:
    int createMessageQueue (key_t key);
};

#endif //LINDACOMMUNICATION_SERVER_H
