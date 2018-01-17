//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_FILEREQUESTMESSAGE_H
#define LINDACOMMUNICATION_FILEREQUESTMESSAGE_H

#include "../../shared/linda/tupleTemplate.h"
#include "../../shared/messages/shared.h"

enum FileOperation {Read, Input, Output};

typedef struct FileRequestMessage
{
    long PID;
    TupleTemplate tupleTemplate;
    FileOperation operation;
    char tuple [TUPLE_MAX_SIZE];
    unsigned int tupleSize;
} FileRequestMessage;

#endif //LINDACOMMUNICATION_FILEREQUESTMESSAGE_H
