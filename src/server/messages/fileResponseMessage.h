//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_FILERESPONSEMESSAGE_H
#define LINDACOMMUNICATION_FILERESPONSEMESSAGE_H

#include "../../shared/messages/shared.h"

enum FileResponseError {TupleNotFound = -1, FileResponseOK = 0};

typedef struct FileResponseMessage
{
    long mtype;
    char tuple [TUPLE_MAX_SIZE];
    int errorCode;

} FileResponseMessage;

#endif //LINDACOMMUNICATION_FILERESPONSEMESSAGE_H
