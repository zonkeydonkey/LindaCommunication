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
    if(msgrcv(requestFileQueueId, &msg, sizeof(msg) - sizeof(long), 0, 0) < 0)
    {
        return -1;
    }
    std::cout << "File Worker - received message" << std::endl;

    switch(msg.operation)
    {
        case Output:
        {
            return outputService(&msg);
        }
        case Input:
        {
            return inputService(&msg);
        }
        case Read:
        {
            return readService(&msg);
        }
    }
    std::cout << "File Worker - Unknown operation" << std::endl;
    return -1;
}

int FileWorker::outputService(FileRequestMessage *msg)
{
    //FileResponseMessage resMsg;
    //resMsg.PID = msg->PID;
    //resMsg.threadID = Out;
    if(printToFile(msg->tuple, msg->tupleSize) == -1)
    {
        std::cout << "File Worker - cannot save tuple in the tuple space" << std::endl;
        //resMsg.errorCode = OutputError;
    }
    else
    {
        std::cout << "File Worker - save tuple in the tuple space:" << std::endl;
        //resMsg.errorCode = FileResponseOK;
    }
    /*if (msgsnd(responseFileQueueId, &resMsg, sizeof(resMsg) - sizeof(long), IPC_NOWAIT) < 0)
    {
        perror("File Worker - message sending error: ");
        return -1;
    }*/
    //printSendMsgInfo(&resMsg);
    return 0;
}

int FileWorker::inputService(FileRequestMessage *msg)
{
    FileResponseMessage resMsg;
    const char *tupleBuffer;
    resMsg.PID = msg->PID;
    resMsg.threadID = In;
    while((tupleBuffer = readFromFile()) != NULL)
    {
        tuple deserializedTuple = deserializeTuple(tupleBuffer);
        if(cmpToTupleTemplate(&deserializedTuple, &(msg->tupleTemplate)) == 0)
        {
            if(removeTupleFromFile() == 0)
            {
                memcpy(resMsg.tuple, tupleBuffer, readedTupleBytesCount);
                resMsg.tupleSize = readedTupleBytesCount;
                resMsg.errorCode = FileResponseOK;
            }
            else
                resMsg.errorCode = InputError;
            break;
        }
    }
    if(tupleBuffer == NULL)
    {
        resMsg.errorCode = TupleNotFound;
    }
    filePos = 0;
    readedTupleBytesCount = 0;
    if (msgsnd(responseFileQueueId, &resMsg, sizeof(resMsg) - sizeof(long), IPC_NOWAIT) < 0)
    {
        perror("File Worker - message sending error: ");
        return -1;
    }
    printSendMsgInfo(&resMsg);
    return 0;
}

int FileWorker::readService(FileRequestMessage *msg)
{
    FileResponseMessage resMsg;
    const char *tupleBuffer;
    resMsg.PID = msg->PID;
    resMsg.threadID = In;
    while((tupleBuffer = readFromFile()) != NULL)
    {
        tuple deserializedTuple = deserializeTuple(tupleBuffer);
        if(cmpToTupleTemplate(&deserializedTuple, &(msg->tupleTemplate)) == 0)
        {
            memcpy(resMsg.tuple, tupleBuffer, readedTupleBytesCount);
            resMsg.tupleSize = readedTupleBytesCount;
            resMsg.errorCode = FileResponseOK;
            break;
        }
    }
    if(tupleBuffer == NULL)
    {
        resMsg.errorCode = TupleNotFound;
    }
    filePos = 0;
    readedTupleBytesCount = 0;
    if (msgsnd(responseFileQueueId, &resMsg, sizeof(resMsg) - sizeof(long), IPC_NOWAIT) < 0)
    {
        perror("File Worker - message sending error: ");
        return -1;
    }
    printSendMsgInfo(&resMsg);
    return 0;
}

int FileWorker::printToFile(char *tupleBuffer, unsigned bytesCount)
{
    std::fstream file;
    file.open(tupleSpaceFile, std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary);
    if(file.is_open())
    {
        file.write(tupleBuffer, bytesCount);
        file << "\n";
        file.close();
        return 0;
    }
    else
        return -1;
}

const char *FileWorker::readFromFile()
{
    std::fstream file;
    file.open(tupleSpaceFile, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    if(file.is_open())
    {
        file.seekg(filePos);
        std::string tupleBuffer;
        std::getline(file, tupleBuffer);
        int currPos = file.tellg();
        if(currPos == -1)
            return NULL;
        readedTupleBytesCount = currPos - filePos - 1;
        filePos = currPos;
        file.close();
        return tupleBuffer.c_str();
    }
    else
    {
        return NULL;
    }
}

int FileWorker::removeTupleFromFile()
{
    std::string tempFileName = "temp";
    std::fstream file;
    std::fstream tempFile;
    file.open(tupleSpaceFile, std::ios_base::in | std::ios_base::binary);
    tempFile.open(tempFileName, std::ios_base::out | std::ios_base::binary);
    if(file.is_open() && tempFile.is_open())
    {
        file.seekg(filePos - 1 - readedTupleBytesCount);
        std::string tupleBuffer;
        std::getline(file, tupleBuffer);
        file.seekg(0);
        std::string currLine;
        // when we have two the same tuple, we remove one
        bool deletedLine = false;
        while(getline(file, currLine))
        {
            if(currLine != tupleBuffer || deletedLine)
            {
                tempFile << currLine << "\n";
            }
            else
                deletedLine = true;
        }
        file.close();
        tempFile.close();
        remove(tupleSpaceFile.c_str());
        rename(tempFileName.c_str(), tupleSpaceFile.c_str());
        return 0;
    }
    return -1;
}

void FileWorker::printSendMsgInfo(FileResponseMessage *resMsg)
{
    std::cout << "File Worker - sended message:" << std::endl;
    std::cout << "PID: " << resMsg->PID;
    std::cout << " | ThreadID: ";
    switch(resMsg->threadID)
    {
        case Out:
        {
            std::cout << "Output";
            break;
        }
        case In:
        {
            std::cout << "Input/Read";
            break;
        }
        std::cout << std::endl;
    }
    //if(resMsg->tuple != NULL)
        //std::cout << "Tuple: " << resMsg->tuple;
    std::cout << " | ErrorCode: ";
    switch(resMsg->errorCode)
    {
        case FileResponseOK:
        {
            std::cout << "OK";
            break;
        }
        case TupleNotFound:
        {
            std::cout << "Tuple not found";
            break;
        }
        case OutputError:
        {
            std::cout << "Output error";
        }
    }
    std::cout << std::endl;
}
