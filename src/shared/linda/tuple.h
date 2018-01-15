#ifndef TUPLE_TUPLE_H
#define TUPLE_TUPLE_H

#include <stdlib.h>
#include <cstdarg>
#include "tupleElement.h"

#define INT_TYPE_CHAR 'i'
#define STRING_TYPE_CHAR 's'


typedef struct tuple
{
    unsigned int elementsCount;
    struct tupleElement *elements;
} tuple;

/*
    To create tuple, using variable arguments list
*/
tuple *makeTuple(std::string elementsTypesList, ...);
/*
    To allocate memory for tuple object and set types for tuple elements
*/
tuple *initializeTuple(std::string elementsTypesList);

#endif //TUPLE_TUPLE_H
