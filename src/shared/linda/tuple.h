#ifndef TUPLE_TUPLE_H
#define TUPLE_TUPLE_H

#include <iostream>
#include <stdlib.h>
#include <cstdarg>
#include <cstddef>
#include <string>
#include <list>

#define INT_TYPE 'i'
#define STRING_TYPE 's'

typedef struct stringElement
{
    std::string value;
    unsigned int idx;
} stringElement;

typedef struct intElement
{
    int value;
    unsigned int idx;
} intElement;

typedef struct tuple
{
    std::list<stringElement> stringElements;
    std::list<intElement> intElements;
} tuple;

/*
    To create tuple, using variable arguments list
*/
tuple makeTuple(std::string elementsTypesList, ...);
/*
    To allocate memory for tuple object
*/
void freeTuple(tuple *tuple);
/*
    To print data associated with tuple
*/
void printTuple(tuple *tuple);

#endif //TUPLE_TUPLE_H
