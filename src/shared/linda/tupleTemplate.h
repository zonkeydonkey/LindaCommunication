//
// Created by sylwia on 1/15/18.
//

#ifndef LINDACOMMUNICATION_TUPLE_H
#define LINDACOMMUNICATION_TUPLE_H

#include <list>
#include <string>
#include <cstring>
#include "../messages/shared.h"

#define MAX_ELEMENTS 10

enum NumberOperator {Equals, Less, LessOrEquals, Greater, GreaterOrEquals};

typedef struct NumberTemplate
{
    NumberOperator numberOperator;
    int from;
    int to;
    int value;
    int order;
} NumberTemplate;

typedef struct TextTemplate
{
    char value[MAX_TEXT_SIZE];
    int order;
    void setValues(int o, std::string val)
    {
        order = o;
        strcpy(value, val.c_str());
    }
} TextTemplate;

typedef struct TupleTemplate
{
    int numberNb;
    int textNb;
    TextTemplate texts [MAX_ELEMENTS];
    NumberTemplate numbers [MAX_ELEMENTS];
} TupleTemplate;

#endif //LINDACOMMUNICATION_TUPLE_H
