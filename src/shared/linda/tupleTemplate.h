//
// Created by sylwia on 1/15/18.
//

#ifndef LINDACOMMUNICATION_TUPLE_H
#define LINDACOMMUNICATION_TUPLE_H

#include <list>
#include <string>

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
    std::string value;
    int order;
} TextTemplate;

typedef struct TupleTemplate
{
    TextTemplate texts [MAX_ELEMENTS];
    NumberTemplate numbers [MAX_ELEMENTS];
} TupleTemplate;

#endif //LINDACOMMUNICATION_TUPLE_H
