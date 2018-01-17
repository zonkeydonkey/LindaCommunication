//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_FILERESPONSEMESSAGE_H
#define LINDACOMMUNICATION_FILERESPONSEMESSAGE_H

#include "../../shared/linda/tuple.h"
#include "../../shared/messages/shared.h"

enum FileResponseError {InputError = -3, OutputError = -2, TupleNotFound = -1, FileResponseOK = 0};
enum ThreadID {Out, In};

typedef struct FileResponseMessage
{
    long mtype;
    ThreadID threadID;
    char tuple [TUPLE_MAX_SIZE];
    int errorCode;
} FileResponseMessage;

#endif //LINDACOMMUNICATION_FILERESPONSEMESSAGE_H
