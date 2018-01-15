#ifndef TUPLE_TUPLEELEMENT_H
#define TUPLE_TUPLEELEMENT_H

#include <stdint.h>
#include <string.h>
#include <string>

typedef struct tupleElement
{
    uint16_t type;
    union
    {
        int i;
        char *s;
    } data;
} tupleElement;

#endif //TUPLE_TUPLEELEMENT_H
