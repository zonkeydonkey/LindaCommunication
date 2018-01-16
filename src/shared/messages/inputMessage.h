//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_INPUTMESSAGE_H
#define LINDACOMMUNICATION_INPUTMESSAGE_H

#include "../linda/tupleTemplate.h"

struct InputMessage
{
    long mtype;
    long PID;
    TupleTemplate tupleTemplate;
    bool isRead;
    time_t timestamp;
    time_t timeout;
};

#endif //LINDACOMMUNICATION_INPUTMESSAGE_H
