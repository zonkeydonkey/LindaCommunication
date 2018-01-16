//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_FILERESPONSEMESSAGE_H
#define LINDACOMMUNICATION_FILERESPONSEMESSAGE_H

#include "../../shared/linda/tuple.h"
#include "../../shared/messages/shared.h"

enum ThreadID {Out, In};

typedef struct FileResponseMessage
{
    long msgType;
    ThreadID threadID;
    tuple foundTuple;
    int errorCode;
} FileResponseMessage;

#endif //LINDACOMMUNICATION_FILERESPONSEMESSAGE_H
