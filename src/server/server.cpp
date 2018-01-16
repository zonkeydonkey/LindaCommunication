//
// Created by sylwia on 1/14/18.
//

#include "server.h"

const std::string Server::sharedConfFilename = "../LindaCommunication/src/shared/conf/queue.conf";
const std::string Server::serverConfFilename = "../LindaCommunication/src/server/conf/queue.conf";
const std::string Server::tupleSpaceConfFilename = "../LindaCommunication/src/server/conf/tupleSpace.conf";

Server::Server()
{
}

Server::~Server()
{
    msgctl(inputQueueId, IPC_RMID, nullptr);
    msgctl(outputQueueId, IPC_RMID, nullptr);
    msgctl(responseQueueId, IPC_RMID, nullptr);
    msgctl(requestFileQueueId, IPC_RMID, nullptr);
    msgctl(responseFileQueueId, IPC_RMID, nullptr);
}

int Server::init()
{
    try
    {
        std::shared_ptr<ConfFile> sharedConf = std::make_shared<ConfFile> (sharedConfFilename);
        std::shared_ptr<ConfFile> serverConf = std::make_shared<ConfFile> (serverConfFilename);
        std::shared_ptr<ConfFile> tupleSpaceConf = std::make_shared<ConfFile> (tupleSpaceConfFilename);

        std::string inputQueId = sharedConf->getProperty("input");
        std::string outputQueId = sharedConf->getProperty("output");
        std::string responseQueId = sharedConf->getProperty("response");
        std::string inputMaxSize = sharedConf->getProperty("inputMaxSize");
        std::string outputMaxSize = sharedConf->getProperty("outputMaxSize");
        std::string reqFileQueId = serverConf->getProperty("requestFile");
        std::string resFileQueId = serverConf->getProperty("responseFile");

        inputQueueId = createMessageQueue (std::stoi(inputQueId));
        outputQueueId = createMessageQueue (std::stoi(outputQueId));
        responseQueueId = createMessageQueue (std::stoi(responseQueId));
        requestFileQueueId = createMessageQueue (std::stoi(reqFileQueId));
        responseFileQueueId = createMessageQueue (std::stoi(resFileQueId));
        tupleSpaceFile = tupleSpaceConf->getProperty("tupleSpaceFile");
        inputMessageMaxSize = (size_t) std::stoi(inputMaxSize);
        outputMessageMaxSize = (size_t) std::stoi(outputMaxSize);
    }
    catch (std::string ex)
    {
        std::cerr << ex << std::endl;
        return -1;
    }
    return 0;
}

int Server::createMessageQueue (key_t key)
{
    int result = msgget (key, QUEUE_PERMS | IPC_CREAT);
    if (result < 0)
    {
        throw std::string ("Error while creating message queue with key: " + key);
    }
    return result;
}

void Server::stop()
{
    running = false;
}

void * inputQueueThreadHandler (void * arg)
{
    Server * server = static_cast<Server *> (arg);




    // TODO
    return nullptr;
}

void * outputQueueThreadHandler (void * arg)
{
    Server * server = static_cast<Server *> (arg);
    OutputMessage message;
    while (server->running)
    {
        if (msgrcv(server->outputQueueId, &message, server->outputMessageMaxSize, 0, 0) < 0)
        {
            std::cerr << "Error while reading from output message queue\n";
            server->stop();
            return server;
        }

        server->processOutputMessage(message);
    }

    return server;
}

void *fileWorkerThreadHandler (void * server)
{
    Server *servPtr = static_cast<Server *>(server);
    FileWorker *fileWorker = new FileWorker(servPtr->requestFileQueueId, servPtr->responseFileQueueId,
                                            servPtr->tupleSpaceFile);
    while(servPtr->running)
    {
        if (fileWorker->receiveMessage() == -1)
            servPtr->stop();
    }

    delete fileWorker;
    return nullptr;
}

void Server::run ()
{
    pthread_t inputMessagesThread;
    pthread_t outputMessagesThread;
    pthread_t fileWorkerThread;

    if(pthread_create(&inputMessagesThread, nullptr, &inputQueueThreadHandler, this)) {
        std::cerr << "Error creating input messages thread\n";
        return;
    }

    if(pthread_create(&outputMessagesThread, nullptr, &outputQueueThreadHandler, this)) {
        std::cerr << "Error creating output messages thread\n";
        return;
    }

    if(pthread_create(&fileWorkerThread, nullptr, &fileWorkerThreadHandler, this)) {
        std::cerr << "Error creating file worker thread\n";
        return;
    }

    //OutputMessage message;
    //processOutputMessage(message);

    pthread_join(inputMessagesThread, nullptr);
    pthread_join(outputMessagesThread, nullptr);
    pthread_join(fileWorkerThread, nullptr);
}

void Server::processOutputMessage(OutputMessage &message)
{
    FileRequestMessage fileRequest;
    fileRequest.operation = Output;
    std::strcpy(fileRequest.tuple, message.tuple);
    size_t messageSize = sizeof(fileRequest);

    if (msgsnd(requestFileQueueId, &fileRequest, messageSize, IPC_NOWAIT) < 0) {
        std::cerr << "An attempt to send request to file worker has failed. Operation: output, tuple: "
                  << fileRequest.tuple << ", message size: " << messageSize << std::endl;
        stop();
    }
}
