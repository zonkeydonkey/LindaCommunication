//
// Created by sylwia on 1/14/18.
//

#include "confFile.h"

ConfFile::ConfFile(const std::string & filenameStr)
{
    const char * filename = filenameStr.c_str();
    file = new std::ifstream (filename);
    if (!file->is_open())
    {
        throw std::string("Cannot open configuration file: " + filenameStr);
    }
}

ConfFile::~ConfFile()
{
    file->close();
    delete file;
}

const std::string ConfFile::getProperty(const std::string & propertyName)
{
    std::string line;
    std::string word;
    while (getline (*file, line))
    {
        std::stringstream ss (line);
        ss >> word;
        if (word == propertyName)
        {
            ss >> word;
            ss >> word;
            setAtBeginning();
            return word;
        }
    }
    setAtBeginning();
    throw "Configuration file does not contain given property";
}

void ConfFile::setAtBeginning ()
{
    file->clear();
    file->seekg(0, std::ios::beg);
}