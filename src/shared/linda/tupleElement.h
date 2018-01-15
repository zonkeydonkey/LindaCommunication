#ifndef TUPLE_TUPLEELEMENT_H
#define TUPLE_TUPLEELEMENT_H

#include <stdint.h>
#include <string>

#define INVALID_TYPE 0x00
#define INT_TYPE 0x01
#define STRING_TYPE 0x02

typedef struct tupleElement
{
    uint16_t type;
    union
    {
        int i;
        std::string s;
    } data;
} tupleElement;

#endif //TUPLE_TUPLEELEMENT_H
