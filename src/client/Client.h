#ifndef LINDACOMMUNICATION_CLIENT_H
#define LINDACOMMUNICATION_CLIENT_H
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <memory>
#include<ctime>
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

    int readResponseMessage() 
    {
    	ResponseMessage message;
		if (msgrcv(responseQueueId, &message, sizeof(message), getpid(), MSG_NOERROR) < 0)
		{
			perror("Błąd - nie udało się pobrać krotki zgodnej ze wzorcem :");
			return -1;
		}
		tuple t = deserializeTuple(message.tuple);
		std::cout << "Udało się znaleźć krotkę: ";
		printTuple(&t);
		return 0;
    }

	int readOrInputMessage(Atom commandType, TupleTemplate templ, time_t timeout) 
	{
		InputMessage inpMessage;
        inpMessage.priority = 1;
        inpMessage.PID = getpid();
        inpMessage.tupleTemplate = templ;
        inpMessage.isRead = (commandType == readInstr);
        inpMessage.timeout = timeout;
        inpMessage.timestamp = std::time(0);
	    if (msgsnd(inputQueueId, &inpMessage, sizeof(inpMessage) - sizeof(long), IPC_NOWAIT) < 0)
	    {
	        perror("Błąd - nie udało się wysłać komunikatu read/input");
            std::cout << "bl: " << errno << std::endl;
	        return -1;
	    }
	    readResponseMessage();
	    return 0;
	}
public:
	Client()
	{
		if (init() != 0)
			throw "Inicjalizacja środowiska nie powiodła się";
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
	    serializeTuple(&toSave, message.tuple, &size);
	    message.originalSize = TUPLE_MAX_SIZE - size;
	    message.PID = getpid();
	    if (msgsnd(outputQueueId, &message, sizeof(message), IPC_NOWAIT) < 0)
	    {
	        perror("Błąd - krotka nie została umieszczona w przestrzeni krotek :");
	        return -1;
	    }
        std::cout << "Tuple: " << std::endl;
        printTuple(&toSave);
        std::cout << " written." << std::endl;
	    return 0;
	}


	void readMessage(TupleTemplate templ, time_t timeout)
	{
		readOrInputMessage(readInstr, templ, timeout);
	}

	void inputMessage(TupleTemplate templ, time_t timeout)
	{
		readOrInputMessage(input, templ, timeout);
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
						readOrInputMessage(cmd.cmd, cmd.tupleUnion.tt, 0);
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
