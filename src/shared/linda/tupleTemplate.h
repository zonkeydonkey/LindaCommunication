//
// Created by sylwia on 1/15/18.
//

#ifndef LINDACOMMUNICATION_TUPLE_H
#define LINDACOMMUNICATION_TUPLE_H

#include <list>
#include <string>
#include <cstring>
#include <iostream>
#include "../messages/shared.h"

#define MAX_ELEMENTS 10

enum Operator {Any, Equals, Less, LessOrEquals, Greater, GreaterOrEquals};

typedef struct NumberTemplate
{
    Operator tempOp;
    int value;
    int order;
    void setValues(Operator op, int ord, int val)
    {
        tempOp = op;
        order = ord;
        value = val;
    }
    void print() 
    {
        std::cout << order << ". operator nr " << tempOp << ", wartość int: " << value << std::endl;
    }
} NumberTemplate;

typedef struct TextTemplate
{
    Operator tempOp;
    char value[MAX_TEXT_SIZE];
    int order;
    void setValues(Operator op, int ord, std::string val)
    {
        tempOp = op;
        order = ord;
        strcpy(value, val.c_str());
    }
    void print() 
    {
        std::cout << order << ". operator nr " << tempOp << ", wartość string: " << value << std::endl;
    }
} TextTemplate;

typedef struct TupleTemplate
{
    int numberNb;
    int textNb;
    TextTemplate texts [MAX_ELEMENTS];
    NumberTemplate numbers [MAX_ELEMENTS];

    void print()
    {
        int num_i = 0;
        int str_i = 0;
        for (int i = 0; i < numberNb+textNb; i++)
        {
            if (texts[str_i].order == i)
                texts[str_i++].print();
            else
                numbers[num_i++].print();
        }
    }
} TupleTemplate;

#endif //LINDACOMMUNICATION_TUPLE_H
