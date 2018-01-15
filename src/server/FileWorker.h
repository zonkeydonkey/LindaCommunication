#ifndef LINDACOMMUNICATION_FILEWORKER_H
#define LINDACOMMUNICATION_FILEWORKER_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include "protocols/fileRequestMessage.h"
#include "protocols/fileResponseMessage.h"

class FileWorker
{
private:
    int requestFileQueueId;
    int responseFileQueueId;
    const std::string tupleSpaceFile;
public:
    FileWorker(int, int, std::string);
    ~FileWorker();

    void receiveMessage();
};

#endif //LINDACOMMUNICATION_FILEWORKER_H
