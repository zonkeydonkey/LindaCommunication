#include "tuple.h"

tuple *makeTuple(std::string elementsTypesList, ...)
{
    // store the list of arguments
    va_list elementsList;
    // initialize elementsList
    va_start(elementsList, elementsTypesList);
    tuple *resultTuple = initializeTuple(elementsTypesList);
    return resultTuple;
}

tuple *initializeTuple(std::string elementsTypesList)
{
    unsigned int elementsCount = elementsTypesList.length();
    tuple *resultTuple = (tuple *)malloc(sizeof(tuple));
    resultTuple->elementsCount = elementsCount;
    resultTuple->elements = (tupleElement *)malloc(sizeof(tupleElement) * elementsCount);
    for(unsigned int i = 0; i < elementsCount; ++i)
    {
        switch(elementsTypesList[i])
        {
            case INT_TYPE_CHAR:
                resultTuple->elements[i].type = INT_TYPE;
                break;
            case STRING_TYPE_CHAR:
                resultTuple->elements[i].type = STRING_TYPE;
                break;
        }

    }
    return resultTuple;
}
