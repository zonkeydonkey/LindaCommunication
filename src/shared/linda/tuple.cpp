#include "tuple.h"

// support macro
#define writeToBuffer(what, buffer, buffer_begin, size, current, type)\
    do{\
        if (*(size) < (int)(sizeof((what)))) {\
            *(buffer_begin) = (current);\
            return (current);\
        }\
        *(type *)(buffer) = (what);\
        (buffer) += sizeof((what));\
        *(size) -= sizeof((what));\
    } while(0)

#define readFromBuffer(what, buffer, type)\
    do {\
        (what) = *(type *)(buffer);\
        (buffer) += sizeof((what));\
    } while (0)

// support functions
static void setTupleElement(va_list *elementsList, unsigned idx, tuple *tuple, char type);
static void setTupleElement(std::string val, unsigned idx, tuple *tuple, char typeChar);

tuple makeTuple(std::string elementsTypesList, ...)
{
    // store the list of arguments
    va_list elementsList;
    // initialize elementsList
    va_start(elementsList, elementsTypesList);
    tuple resultTuple;
    resultTuple.elementsCount = elementsTypesList.length();
    for(unsigned i = 0; i < resultTuple.elementsCount; ++i)
        setTupleElement(&elementsList, i, &resultTuple, elementsTypesList[i]);
    return resultTuple;
}

tuple makeTuple(std::vector<std::string> values, std::string elementsTypesList) 
{
    tuple resultTuple;
    resultTuple.elementsCount = elementsTypesList.length();
    for(unsigned i = 0; i < resultTuple.elementsCount; ++i) 
    {
        setTupleElement(values[i], i, &resultTuple, elementsTypesList[i]);
    }
    return resultTuple;
}

void freeTuple(tuple *tuple)
{
    tuple->intElements.clear();
    tuple->stringElements.clear();
    tuple->elementsCount = 0;
}

void printTuple(tuple *tuple)
{
    unsigned idx = 0;
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

int serializeTuple(const tuple *tuple, char *buffer, int *size)
{
    unsigned *buffer_begin = (unsigned *)buffer;
    unsigned idx = 0;
    writeToBuffer(tuple->elementsCount, buffer, buffer_begin, size, idx, unsigned);
    std::list<intElement>::const_iterator intIter = tuple->intElements.begin();
    for(std::list<stringElement>::const_iterator strIter = tuple->stringElements.begin();
        strIter != tuple->stringElements.end();)
    {
        if((*strIter).idx == idx)
        {
            writeToBuffer((uint8_t)STRING_TYPE, buffer, buffer_begin, size, idx, uint8_t);
            int stringLength = (*strIter).value.length();
            if(*size < stringLength + 1)
            {
                *buffer_begin = idx;
                return idx;
            }
            strcpy(buffer, (*strIter).value.c_str());
            buffer += stringLength + 1;
            *size -= stringLength + 1;
            ++strIter;
        }
        else if((*intIter).idx == idx)
        {
            writeToBuffer((uint8_t)INT_TYPE, buffer, buffer_begin, size, idx, uint8_t);
            writeToBuffer((*intIter).value, buffer, buffer_begin, size, idx, int);
            ++intIter;
        }
        else
            return -1;
        ++idx;
    }
    for(;intIter != tuple->intElements.end(); ++intIter)
    {
        writeToBuffer((uint8_t)INT_TYPE, buffer, buffer_begin, size, idx, uint8_t);
        writeToBuffer((*intIter).value, buffer, buffer_begin, size, idx, int);
        ++idx;
    }
    return idx;
}

tuple deserializeTuple(const char *buffer)
{
    tuple resultTuple;
    uint8_t elementType;
    readFromBuffer(resultTuple.elementsCount, buffer, unsigned);
    for(unsigned idx = 0; idx < resultTuple.elementsCount; ++idx)
    {
        readFromBuffer(elementType, buffer, uint8_t);
        switch(elementType)
        {
            case STRING_TYPE:
            {
                stringElement element;
                int stringLength = strlen(buffer);
                char *tupleValue = (char *)malloc(stringLength + 1);
                strcpy(tupleValue ,buffer);
                buffer += stringLength + 1;
                element.value = std::string(tupleValue);
                element.idx = idx;
                resultTuple.stringElements.push_back(element);
                break;
            }
            case INT_TYPE:
            {
                intElement element;
                int tupleValue;
                readFromBuffer(tupleValue, buffer, int);
                element.value = tupleValue;
                element.idx = idx;
                resultTuple.intElements.push_back(element);
                break;
            }
            default:
            {
                freeTuple(&resultTuple);
                return resultTuple;
            }
        }
    }
    return resultTuple;
}


static void setTupleElement(va_list *elementsList, unsigned idx, tuple *tuple, char typeChar)
{
    switch(typeChar)
    {
        case INT_TYPE_CHAR:
        {
            intElement element;
            element.value = va_arg(*elementsList, int);;
            element.idx = idx;
            tuple->intElements.push_back(element);
            break;
        }
        case STRING_TYPE_CHAR:
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

static void setTupleElement(std::string val, unsigned idx, tuple *tuple, char typeChar)
{
    switch(typeChar)
    {
        case INT_TYPE_CHAR:
        {
            intElement element;
            element.value = std::stoi(val);
            element.idx = idx;
            tuple->intElements.push_back(element);
            break;
        }
        case STRING_TYPE_CHAR:
        {
            stringElement element;
            element.value = val;
            element.idx = idx;
            tuple->stringElements.push_back(element);
            break;
        }
    }
}