//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_SERVER_H
#define LINDACOMMUNICATION_SERVER_H

#include <string>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <errno.h>

#include "FileWorker.h"
#include "../shared/utils/confFile.h"
#include "../shared/messages/outputMessage.h"
#include "../shared/messages/shared.h"
#include "messages/fileRequestMessage.h"
#include "../shared/messages/inputMessage.h"
#include "../shared/messages/responseMessage.h"
#include "messages/fileResponseMessage.h"

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

protected:
    void processOutputMessage(OutputMessage &message);
    void sendTimeoutedInfo(long PID);
    void processHighPriorityInput(InputMessage &message);
    FileResponseMessage tryFindTuple(InputMessage &inputMessage);
    void sendTupleFoundInfo(FileResponseMessage &fileResponseMessage);
    void sendBackTuple(FileResponseMessage &fileResponseMessage);
    void processInputMessage(InputMessage &message);
    void sendBackInputMessage(InputMessage &previous);
    void processTupleFound(InputMessage &inputMessage, FileResponseMessage &responseMessage);
};

#endif //LINDACOMMUNICATION_SERVER_H
