#ifndef LINDACOMMUNICATION_CLIENT_H
#define LINDACOMMUNICATION_CLIENT_H
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <memory>
#include <iostream>
#include "Parse.h"
#include "../shared/messages/outputMessage.h" //tuples
#include "../shared/messages/inputMessage.h" // templates
#include "../shared/messages/responseMessage.h"
#include "../shared/utils/confFile.h"


class Client {
	Parse *parse;
	const std::string sharedConfFilename = "./src/shared/conf/queue.conf";
	int inputQueueId;
    int outputQueueId;
    int responseQueueId;
public:
	Client() 
	{
		if (init() != 0)
			throw "Inicjalizacja środowieska nie powiodła się";
		parse = new Parse();
	}

	~Client() 
	{
		delete parse;
	}

int init()
{
    try
    {
        std::shared_ptr<ConfFile> sharedConf = std::make_shared<ConfFile> (sharedConfFilename);
        std::string inputQueId = sharedConf->getProperty("input");
        std::string outputQueId = sharedConf->getProperty("output");
        std::string responseQueId = sharedConf->getProperty("response");
        inputQueueId = openMessageQueue (std::stoi(inputQueId));
        outputQueueId = openMessageQueue (std::stoi(outputQueId));
        responseQueueId = openMessageQueue (std::stoi(responseQueId));
    }
    catch (char const* ex)
    {
        std::cerr << ex << std::endl;
        return -1;
    }
    return 0;
}

int openMessageQueue (key_t key)
{
    int result = msgget (key, QUEUE_PERMS );
    if (result < 0)
    {
        throw "Operacja otwierania kolejki nie powiodła się";
    }
    return result;
}

	int outputMessage(tuple toSave) 
	{
		OutputMessage message;
	    int size = TUPLE_MAX_SIZE;
	    size_t sizeBytes = serializeTuple(&toSave, message.tuple, &size);
	    message.originalSize = sizeBytes;
	    message.PID = getpid();
	    if (msgsnd(outputQueueId, &message, sizeof(message), IPC_NOWAIT) < 0)
	    {
	        perror("Błąd - krotka nie została umieszczona w przestrzeni krotek");
	        return -1;
	    }
	    std::cout << "Krotka umieszczona w przestrzeni\n";
	    return 0;
	}

	void inputMessage(TupleTemplate templ, int itmeout)
	{

	}

	void readMessage(TupleTemplate templ, int itmeout)
	{

	}

	void run() 
	{
		while (true)
		{
			try {
				Command cmd = parse->parseCommand();
				switch (cmd.cmd) 
				{
					case output:
						outputMessage(cmd.tupleUnion.t);
						break;
					case input:
					case readInstr:
						break;
					default:
						break;
				}
			} catch (const char * str) {
				std::cout << str << std::endl;
			}
		}
	}
};

#endif