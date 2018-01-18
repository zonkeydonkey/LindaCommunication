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
static int checkStringValue(stringElement tupleElement, TextTemplate templateElement);
static int checkIntegerValue(intElement tupleElement, NumberTemplate templateElement);

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

int getTupleSize(const tuple *tuple)
{
    int size = 0;
    unsigned idx = 0;
    size += sizeof(tuple->elementsCount);
    std::list<intElement>::const_iterator intIter = tuple->intElements.begin();
    for(std::list<stringElement>::const_iterator strIter = tuple->stringElements.begin();
        strIter != tuple->stringElements.end();)
    {
        if((*strIter).idx == idx)
        {
            size += sizeof((uint8_t)STRING_TYPE);
            int stringLength = (*strIter).value.length();
            size += stringLength + 1;
            ++strIter;
        }
        else if((*intIter).idx == idx)
        {
            size += sizeof((uint8_t)INT_TYPE);
            size += sizeof((*intIter).value);
            ++intIter;
        }
        else
            return -1;
        ++idx;
    }
    for(;intIter != tuple->intElements.end(); ++intIter)
    {
        size += sizeof((uint8_t)INT_TYPE);
        size += sizeof((*intIter).value);
        ++idx;
    }
    return size;
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

int cmpToTupleTemplate(const tuple *tuple, const TupleTemplate *tupleTemplate)
{
    if((tuple->stringElements.size() != (unsigned)tupleTemplate->textNb) ||
       (tuple->intElements.size() != (unsigned)tupleTemplate->numberNb))
        return -1;
    unsigned i = 0;
    for(std::list<stringElement>::const_iterator strIter = tuple->stringElements.begin();
        strIter != tuple->stringElements.end(); ++strIter)
    {
        if((*strIter).idx != (unsigned)tupleTemplate->texts[i].order)
            return -1;
        if(checkStringValue((*strIter), tupleTemplate->texts[i]) < 0)
            return -1;
        ++i;
    }
    i = 0;
    for(std::list<intElement>::const_iterator intIter = tuple->intElements.begin();
        intIter != tuple->intElements.end(); ++intIter)
    {
        if((*intIter).idx != (unsigned)tupleTemplate->numbers[i].order)
            return -1;
        if(checkIntegerValue((*intIter), tupleTemplate->numbers[i]) < 0)
            return -1;
        ++i;
    }
    return 0;
}

static int checkStringValue(stringElement tupleElement, TextTemplate templateElement)
{
    std::string tupleValue = tupleElement.value;
    std::string templateValue = std::string(templateElement.value);
    switch(templateElement.tempOp)
    {
        case Any:
        {
            return 0;
        }
        case Equals:
        {
            size_t anyPos;
            if((anyPos = templateValue.find("*")) != std::string::npos)
            {
                if((anyPos == 0) || (tupleValue.substr(0, anyPos) == templateValue.substr(0, anyPos)))
                {
                    int tupleValueLength = tupleValue.length();
                    int templateValueLength = templateValue.length();
                    int secondPartTemplateValueLength = templateValueLength - anyPos - 1;
                    if(secondPartTemplateValueLength == 0)
                        return 0;
                    std::string secondPartTemplateValue = templateValue.substr(anyPos + 1, secondPartTemplateValueLength);
                    if(tupleValue.substr(tupleValueLength - secondPartTemplateValueLength, secondPartTemplateValueLength) == secondPartTemplateValue)
                        return 0;
                }
                break;
            }
            else
            {
                if(tupleValue == templateValue)
                    return 0;
            }
            break;
        }
        case Less:
        {
            if(tupleValue < templateValue)
                return 0;
            break;
        }
        case LessOrEquals:
        {
            if(tupleValue <= templateValue)
                return 0;
            break;
        }
        case Greater:
        {
            if(tupleValue > templateValue)
                return 0;
            break;
        }
        case GreaterOrEquals:
        {
            if(tupleValue >= templateValue)
                return 0;
            break;
        }
    }
    return -1;
}

static int checkIntegerValue(intElement tupleElement, NumberTemplate templateElement)
{
    int tupleValue = tupleElement.value;
    int templateValue = templateElement.value;
    switch(templateElement.tempOp)
    {
        case Any:
        {
            return 0;
        }
        case Equals:
        {
            if(tupleValue == templateValue)
                return 0;
            break;
        }
        case Less:
        {
            if(tupleValue < templateValue)
                return 0;
            break;
        }
        case LessOrEquals:
        {
            if(tupleValue <= templateValue)
                return 0;
            break;
        }
        case Greater:
        {
            if(tupleValue > templateValue)
                return 0;
            break;
        }
        case GreaterOrEquals:
        {
            if(tupleValue >= templateValue)
                return 0;
            break;
        }
    }
    return -1;
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
