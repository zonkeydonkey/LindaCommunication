#include "FileWorker.h"

FileWorker::FileWorker(int _requestFileQueueId, int _responseFileQueueId, const std::string _tupleSpaceFile)
                      : requestFileQueueId(_requestFileQueueId), responseFileQueueId(_responseFileQueueId),
                        tupleSpaceFile(_tupleSpaceFile)
{}

FileWorker::~FileWorker()
{}

int FileWorker::receiveMessage()
{
    FileRequestMessage msg;
    if(msgrcv(requestFileQueueId, &msg, sizeof(msg), 0, 0) < 0)
    {
        perror("File Worker - message receiving error: ");
        return -1;
    }
    std::cout << "File Worker Thread received message." << std::endl;

    switch(msg.operation)
    {
        case Output:
        {
            std::cout << "output" << std::endl;
            break;
        }
        case Input:
        {
            std::cout << "input" << std::endl;
            break;
        }
        case Read:
        {
            std::cout << "read" << std::endl;
            break;
        }
    }

    return 0;
}
