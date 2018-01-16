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
    std::cout << "File Worker - received message" << std::endl;

    switch(msg.operation)
    {
        case Output:
        {
            if(printToFile(msg.tuple, msg.tupleBufferBytes) == -1)
            {
                std::cout << "File Worker - cannot save tuple in the tuple space" << std::endl;
                // wyslanie komunikatu o bledzie
            }
            // wyslanie komunikatu ze sie udalo
            break;
        }
        case Input:
        {
            unsigned idx;
            const char *tupleBuffer;
            while((tupleBuffer = readFromFile(idx)) != NULL)
            {
                tuple deserializedTuple = deserializeTuple(tupleBuffer);
                // porównanie z templatem
                if(true)
                {
                    // wyslanie komunikatu
                    return 0;
                }
                ++idx;
            }
            // nie znaleziono krotki wiec odsylamy nulla
            break;
        }
        case Read:
        {
            unsigned idx;
            const char *tupleBuffer;
            while((tupleBuffer = readFromFile(idx)) != NULL)
            {
                tuple deserializedTuple = deserializeTuple(tupleBuffer);
                // porównanie z templatem
                if(true)
                {
                    // wyslanie komunikatu
                    return 0;
                }
                ++idx;
            }
            // nie znaleziono krotki wiec odsylamy nulla
            break;
        }
    }
    return 0;
}

int FileWorker::printToFile(char *tupleBuffer, unsigned bytesCount)
{
    const char* tupleSpaceFilename = tupleSpaceFile.c_str();
    std::ofstream file(tupleSpaceFilename, std::ofstream::app | std::ofstream::binary);
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

const char *FileWorker::readFromFile(unsigned idx)
{
    const char* tupleSpaceFilename = tupleSpaceFile.c_str();
    std::ifstream file(tupleSpaceFilename, std::ofstream::in | std::ofstream::binary);
    if(file.is_open())
    {
        unsigned i = 0;
        while(i < idx)
        {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if(file.eof())
                return NULL;
            ++i;
        }
        std::string tupleBuffer;
        std::getline(file, tupleBuffer);
        file.close();
        return tupleBuffer.c_str();
    }
    else
    {
        return NULL;
    }
}
