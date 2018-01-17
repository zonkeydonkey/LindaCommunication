#ifndef TUPLE_TUPLE_H
#define TUPLE_TUPLE_H

#include <iostream>
#include <stdlib.h>
#include <cstdarg>
#include <cstddef>
#include <string>
#include <string.h>
#include <list>
#include "tupleTemplate.h"
#include <vector>

#define INT_TYPE 0x00
#define STRING_TYPE 0x01
#define INT_TYPE_CHAR 'i'
#define STRING_TYPE_CHAR 's'

typedef struct stringElement
{
    std::string value;
    unsigned idx;
} stringElement;

typedef struct intElement
{
    int value;
    unsigned idx;
} intElement;

typedef struct tuple
{
    unsigned elementsCount;
    std::list<stringElement> stringElements;
    std::list<intElement> intElements;
} tuple;

/*
    To create tuple, using variable arguments list
*/
tuple makeTuple(std::string elementsTypesList, ...);
tuple makeTuple(std::vector<std::string> values, std::string elementsTypesList);
/*
    To allocate memory for tuple object
*/
void freeTuple(tuple *tuple);
/*
    To print data associated with tuple
*/
void printTuple(tuple *tuple);
/*
    To serialize provided tuple.
    Returns count of elements that have been serialized or -1, in case of error.
*/
int serializeTuple(const tuple *tuple, char *buffer, int *size);
/*
    To deserialize tuple.
*/
tuple deserializeTuple(const char *buffer);
/*
    To compare tuple to template.
*/
int cmpToTupleTemplate(const tuple *tuple, const TupleTemplate *tupleTemplate);

#endif //TUPLE_TUPLE_H
