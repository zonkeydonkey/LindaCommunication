//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_OUTPUTMESSAGE_H
#define LINDACOMMUNICATION_OUTPUTMESSAGE_H

#include "shared.h"

typedef struct OutputMessage
{
    long PID;
    char tuple [TUPLE_MAX_SIZE];
    size_t originalSize;
} OutputMessage;

#endif //LINDACOMMUNICATION_OUTPUTMESSAGE_H
