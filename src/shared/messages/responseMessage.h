//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_RESPONSEMESSAGE_H
#define LINDACOMMUNICATION_RESPONSEMESSAGE_H

#include "shared.h"

enum ResponseError {Timeouted = -1, ResponseOK = 0};

typedef struct ResponseMessage
{
    long mtype;
    ResponseError errorCode;
    char tuple [TUPLE_MAX_SIZE];

} ResponseMessage;

#endif //LINDACOMMUNICATION_RESPONSEMESSAGE_H
