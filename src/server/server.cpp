//
// Created by sylwia on 1/14/18.
//

#include "server.h"

const std::string Server::sharedConfFilename = "../LindaCommunication/src/shared/conf/queue.conf";
const std::string Server::serverConfFilename = "../LindaCommunication/src/server/conf/queue.conf";

Server::Server()
{
    init();
}

Server::~Server()
{
    msgctl(inputQueueId, IPC_RMID, nullptr);
    msgctl(outputQueueId, IPC_RMID, nullptr);
    msgctl(responseQueueId, IPC_RMID, nullptr);
    msgctl(requestFileQueueId, IPC_RMID, nullptr);
    msgctl(responseFileQueueId, IPC_RMID, nullptr);
}

void Server::init()
{
    try
    {
        std::shared_ptr<ConfFile> sharedConf = std::make_shared<ConfFile> (sharedConfFilename);
        std::shared_ptr<ConfFile> serverConf = std::make_shared<ConfFile> (serverConfFilename);

        std::string inputQueId = sharedConf->getProperty("input");
        std::string outputQueId = sharedConf->getProperty("output");
        std::string responseQueId = sharedConf->getProperty("response");
        std::string reqFileQueId = serverConf->getProperty("requestFile");
        std::string resFileQueId = serverConf->getProperty("responseFile");

        inputQueueId = createMessageQueue (std::stoi(inputQueId));
        outputQueueId = createMessageQueue (std::stoi(outputQueId));
        responseQueueId = createMessageQueue (std::stoi(responseQueId));
        requestFileQueueId = createMessageQueue (std::stoi(reqFileQueId));
        responseFileQueueId = createMessageQueue (std::stoi(resFileQueId));
    }
    catch (std::string ex)
    {
        std::cerr << ex << std::endl;
    }
}

int Server::createMessageQueue (key_t key)
{
    int result = msgget (key, 0666 | IPC_CREAT);
    if (result < 0)
    {
        throw std::string ("Error while creating message queue with key: " + key);
    }
    return result;
}

void Server::run ()     // TODO - watek zarzadcy pliku
{
    pthread_t inputMessagesThread;
    pthread_t outputMessagesThread;

    if(pthread_create(&inputMessagesThread, nullptr, &inputQueueThreadHandler, this)) {
        std::cerr << "Error creating input messages thread\n";
        return;
    }

    if(pthread_create(&outputMessagesThread, nullptr, &outputQueueThreadHandler, this)) {
        std::cerr << "Error creating output messages thread\n";
        return;
    }

    pthread_join(inputMessagesThread, nullptr);
    pthread_join(outputMessagesThread, nullptr);
}

void * inputQueueThreadHandler (void * server)
{
    return nullptr;
}

void * outputQueueThreadHandler (void * server)
{
    return nullptr;
}

