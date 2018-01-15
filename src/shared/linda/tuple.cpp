#include "tuple.h"

// support functions
static void setTupleElement(va_list *elementsList, unsigned int idx, tuple *tuple, char type);

tuple makeTuple(std::string elementsTypesList, ...)
{
    // store the list of arguments
    va_list elementsList;
    // initialize elementsList
    va_start(elementsList, elementsTypesList);
    tuple resultTuple;
    for(unsigned int i = 0; i < elementsTypesList.length(); ++i)
        setTupleElement(&elementsList, i, &resultTuple, elementsTypesList[i]);
    return resultTuple;
}

void freeTuple(tuple *tuple)
{
    tuple->intElements.clear();
    tuple->stringElements.clear();
}

void printTuple(tuple *tuple)
{
    unsigned int idx = 0;
    std::list<intElement>::iterator intIter = tuple->intElements.begin();
    for(std::list<stringElement>::iterator strIter = tuple->stringElements.begin();
        strIter != tuple->stringElements.end();)
    {
        std::cout << "Element " << idx << ": ";
        if((*strIter).idx == idx)
        {
            std::cout << "Type: string, ";
            std::cout << "Value: " << (*strIter).value << std::endl;
            ++strIter;
        }
        else
        {
            std::cout << "Type: int, ";
            std::cout << "Value: " << (*intIter).value << std::endl;
            ++intIter;
        }
        ++idx;
    }
    for(;intIter != tuple->intElements.end(); ++intIter)
    {
        std::cout << "Element " << idx << ": ";
        std::cout << "Type: int, ";
        std::cout << "Value: " << (*intIter).value << std::endl;
        ++idx;
    }
}

static void setTupleElement(va_list *elementsList, unsigned int idx, tuple *tuple, char type)
{
    switch(type)
    {
        case INT_TYPE:
        {
            intElement element;
            element.value = va_arg(*elementsList, int);;
            element.idx = idx;
            tuple->intElements.push_back(element);
            break;
        }
        case STRING_TYPE:
        {
            stringElement element;
            char *argValue = va_arg(*elementsList, char *);
            element.value = std::string(argValue);
            element.idx = idx;
            tuple->stringElements.push_back(element);
            break;
        }
    }
}
