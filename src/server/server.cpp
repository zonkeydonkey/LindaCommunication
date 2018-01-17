//
// Created by sylwia on 1/14/18.
//

#include "server.h"

// LindaCommunication
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
        std::string reqFileQueId = serverConf->getProperty("requestFile");
        std::string resFileQueId = serverConf->getProperty("responseFile");

        inputQueueId = createMessageQueue (std::stoi(inputQueId));
        outputQueueId = createMessageQueue (std::stoi(outputQueId));
        responseQueueId = createMessageQueue (std::stoi(responseQueId));
        requestFileQueueId = createMessageQueue (std::stoi(reqFileQueId));
        responseFileQueueId = createMessageQueue (std::stoi(resFileQueId));
        tupleSpaceFile = tupleSpaceConf->getProperty("tupleSpaceFile");
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

void Server::sendTimeoutedInfo(long PID)
{
    ResponseMessage response;
    response.mtype = PID;
    response.errorCode = Timeouted;

    if (msgsnd(responseQueueId, &response, sizeof(response) - sizeof(long), IPC_NOWAIT) < 0)
    {
        std::cerr << "An attempt to send response (timeouted info) has failed." << std::endl;
        stop();
    }
}

FileResponseMessage Server::tryFindTuple(InputMessage &inputMessage)
{
    FileRequestMessage fileRequest;
    fileRequest.mtype = inputMessage.PID;
    fileRequest.tupleTemplate = inputMessage.tupleTemplate;
    fileRequest.operation = inputMessage.isRead ? Read : Input;

    if (msgsnd(requestFileQueueId, &fileRequest, sizeof(fileRequest) - sizeof(long), IPC_NOWAIT) < 0)
    {
        std::cerr << "An attempt to send request to file worker has failed. Operation: input/read" << std::endl;
        stop();
    }

    FileResponseMessage fileResponse;

    if (msgrcv(responseFileQueueId, &fileResponse, FILE_RESPONSE_MAX_SIZE, inputMessage.PID, 0) < 0)
    {
        std::cerr << "Error while attempting to get response from file worker for process with PID: " << inputMessage.PID << std::endl;
        stop();
    }

    return fileResponse;
}

void Server::sendTupleFoundInfo (FileResponseMessage &fileResponseMessage)
{
    ResponseMessage responseMessage;
    std::strcpy(responseMessage.tuple, fileResponseMessage.tuple);
    responseMessage.errorCode = ResponseError::ResponseOK;
    responseMessage.mtype = fileResponseMessage.mtype;

    if (msgsnd(responseQueueId, &responseMessage, sizeof(responseMessage), IPC_NOWAIT) < 0) {
        std::cerr << "An attempt to send response with tuple has failed. Process PID: " << responseMessage.mtype
                << ", tuple: " << responseMessage.tuple << std::endl;
        stop();
    }
}

void Server::sendBackTuple(FileResponseMessage &fileResponseMessage)
{
    OutputMessage outputMessage;
    std::strcpy(outputMessage.tuple, fileResponseMessage.tuple);
    outputMessage.PID = fileResponseMessage.mtype;

    if (msgsnd(outputQueueId, &outputMessage, sizeof(outputMessage), IPC_NOWAIT) < 0) {
        std::cerr << "An attempt to resend message with tuple has failed. Process PID: " << outputMessage.PID
                  << ", tuple: " << outputMessage.tuple << std::endl;
        stop();
    }
}

void Server::processHighPriorityInput(InputMessage &inputMessage)
{
    while (true)
    {
        if (std::time(0) > (inputMessage.timestamp + inputMessage.timeout))
        {
            sendTimeoutedInfo(inputMessage.PID);
            return;
        }
        else
        {
            FileResponseMessage responseMessage = tryFindTuple(inputMessage);
            if (responseMessage.errorCode == FileResponseOK)
            {
                processTupleFound(inputMessage, responseMessage);
                return;
            }
            else
            {
                sleep(WAIT_SECONDS);
            }
        }
    }
}

void * inputQueueThreadHandler (void * arg)
{
    Server * server = static_cast<Server *> (arg);
    InputMessage inputMessage;

    while (server->running)
    {
        if (msgrcv(server->inputQueueId, &inputMessage, INPUT_MESSAGE_MAX_SIZE, HIGHEST_PRIORITY, IPC_NOWAIT) >= 0)
        {
            server->processHighPriorityInput(inputMessage);
        }
        else
        {
            if (msgrcv(server->inputQueueId, &inputMessage, INPUT_MESSAGE_MAX_SIZE, 0, 0) < 0)
            {
                std::cerr << "Error while reading from input message queue\n";
                server->stop();
                return server;
            }
            server->processInputMessage(inputMessage);
        }
    }
    return server;
}

void * outputQueueThreadHandler (void * arg)
{
    Server * server = static_cast<Server *> (arg);
    OutputMessage message;

    while (server->running)
    {
        if (msgrcv(server->outputQueueId, &message, OUTPUT_MESSAGE_MAX_SIZE, 0, 0) < 0)
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
        if (fileWorker->receiveMessage() < 0)
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

    std::cout << "Server init." << std::endl;

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

    /* do testow
    OutputMessage message;
    tuple myTuple = makeTuple("sis", "lol", 1, "xD");
    int size = TUPLE_MAX_SIZE;
    serializeTuple(&myTuple, message.tuple, &size);
    message.PID = 12;
    processOutputMessage(message);
    */

    pthread_join(inputMessagesThread, nullptr);
    pthread_join(outputMessagesThread, nullptr);
    pthread_join(fileWorkerThread, nullptr);
}

void Server::processOutputMessage(OutputMessage &message)
{
    FileRequestMessage fileRequest;
    fileRequest.operation = Output;
    memcpy(fileRequest.tuple, message.tuple, sizeof(message.tuple));
    fileRequest.mtype = message.PID;
    // tutaj trzeba zrobić, zeby file worker dostawal info o rzeczywistej ilosci bajtow
    fileRequest.tupleBufferBytes = 18;

    if (msgsnd(requestFileQueueId, &fileRequest, sizeof(fileRequest) - sizeof(long), IPC_NOWAIT) < 0) {
        std::cerr << "An attempt to send request to file worker has failed. Operation: output, tuple: "
                  << fileRequest.tuple << ", process PID: " << fileRequest.mtype << std::endl;
        stop();
    }
}

void Server::processInputMessage(InputMessage &inputMessage)
{
    if (std::time(0) > (inputMessage.timestamp + inputMessage.timeout))
    {
        sendTimeoutedInfo(inputMessage.PID);
    }
    else
    {
        FileResponseMessage responseMessage = tryFindTuple(inputMessage);
        if (responseMessage.errorCode == FileResponseOK)
        {
            processTupleFound(inputMessage, responseMessage);
        }
        else
        {
            sendBackInputMessage(inputMessage);
        }
    }
}

void Server::sendBackInputMessage(InputMessage &previous)
{
    previous.mtype = previous.mtype + 1;

    if (msgsnd(inputQueueId, &previous, sizeof(previous) - sizeof(long), IPC_NOWAIT) < 0) {
        std::cerr << "An attempt to send back input message has failed, process PID: " << previous.PID << std::endl;
        stop();
    }
}

void Server::processTupleFound(InputMessage &inputMessage, FileResponseMessage &responseMessage)
{
    if (std::time(0) > (inputMessage.timestamp + inputMessage.timeout))
    {
        sendTimeoutedInfo(inputMessage.PID);
        sendBackTuple(responseMessage);
    }
    else
    {
        sendTupleFoundInfo(responseMessage);
    }
}
