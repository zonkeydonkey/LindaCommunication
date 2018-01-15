#include "FileWorker.h"

FileWorker::FileWorker(int _requestFileQueueId, int _responseFileQueueId, const std::string _tupleSpaceFile)
                      : requestFileQueueId(_requestFileQueueId), responseFileQueueId(_responseFileQueueId),
                        tupleSpaceFile(_tupleSpaceFile)
{}

FileWorker::~FileWorker()
{}

void FileWorker::receiveMessage()
{
    //std::cout << "FileWorker: wait for request" << std::endl;
    //if(msgrcv(requestFileQueueId, &fileRequestMessage, sizeof()))
}
