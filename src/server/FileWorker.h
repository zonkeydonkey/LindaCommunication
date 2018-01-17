#ifndef LINDACOMMUNICATION_FILEWORKER_H
#define LINDACOMMUNICATION_FILEWORKER_H

#include <iostream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <errno.h>
#include <fstream>
#include <limits>
#include "messages/fileRequestMessage.h"
#include "messages/fileResponseMessage.h"
#include "../shared/linda/tuple.h"


class FileWorker
{
private:
    int requestFileQueueId;
    int responseFileQueueId;
    const std::string tupleSpaceFile;
    int filePos = 0;
    int readedTupleBytesCount = 0;

    int outputService(FileRequestMessage *msg);
    int inputService(FileRequestMessage *msg);
    int readService(FileRequestMessage *msg);
    int printToFile(char *tupleBuffer, unsigned bytesCount);
    const char *readFromFile();
    int removeTupleFromFile();
    void printSendMsgInfo(FileResponseMessage *resMsg);
public:
    FileWorker(int, int, std::string);
    ~FileWorker();

    int receiveMessage();
};

#endif //LINDACOMMUNICATION_FILEWORKER_H
