#include "tuple.h"

// support functions
static void setTupleElement(va_list *elementsList, unsigned int idx, tuple *tuple, char typeChar);

tuple *makeTuple(std::string elementsTypesList, ...)
{
    // store the list of arguments
    va_list elementsList;
    // initialize elementsList
    va_start(elementsList, elementsTypesList);
    unsigned int elementsCount = elementsTypesList.length();
    tuple *resultTuple = initializeTuple(elementsCount);
    for(unsigned int i = 0; i < resultTuple->elementsCount; ++i)
        setTupleElement(&elementsList, i, resultTuple, elementsTypesList[i]);
    return resultTuple;
}

tuple *initializeTuple(unsigned int elementsCount)
{
    tuple *resultTuple = (tuple *)malloc(sizeof(tuple));
    resultTuple->elementsCount = elementsCount;
    resultTuple->elements = (tupleElement *)malloc(sizeof(tupleElement) * elementsCount);
    return resultTuple;
}

void *freeTuple(tuple *tuple)
{
    for(unsigned int i = 0; i < tuple->elementsCount; ++i)
    {
        if(tuple->elements[i].type == STRING_TYPE)
            free(tuple->elements[i].data.s);
    }
    free(tuple->elements);
    free(tuple);
}


void printTuple(tuple *tuple)
{
    for(unsigned int i = 0; i < tuple->elementsCount; ++i)
    {
        std::cout << "Element " << i << ": ";
        switch(tuple->elements[i].type)
        {
            case INT_TYPE:
                std::cout << "Type: int, ";
                std::cout << "Value: " << tuple->elements[i].data.i << std::endl;
                break;
            case STRING_TYPE:
                std::cout << "Type: string, ";
                std::cout << "Value: " << tuple->elements[i].data.s << std::endl;
                break;
        }
    }
}

static void setTupleElement(va_list *elementsList, unsigned int idx, tuple *tuple, char typeChar)
{
    switch(typeChar)
    {
        case INT_TYPE_CHAR:
        {
            tuple->elements[idx].type = INT_TYPE;
            tuple->elements[idx].data.i = va_arg(*elementsList, int);
            break;
        }
        case STRING_TYPE_CHAR:
        {
            tuple->elements[idx].type = STRING_TYPE;
            char *tupleValue = va_arg(*elementsList, char *);
            tuple->elements[idx].data.s = (char *)malloc(strlen(tupleValue) + 1);
            strcpy(tuple->elements[idx].data.s, tupleValue);
            break;
        }
        default:
        {
            tuple->elements[idx].type = INVALID_TYPE;
        }
    }
}
