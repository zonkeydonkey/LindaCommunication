//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_OUTPUTMESSAGE_H
#define LINDACOMMUNICATION_OUTPUTMESSAGE_H

#include "shared.h"

typedef struct OutputMessage
{
    pid_t PID;
    char tuple [MAX_TUPLE_SIZE];
} OutputMessage;

#endif //LINDACOMMUNICATION_OUTPUTMESSAGE_H
