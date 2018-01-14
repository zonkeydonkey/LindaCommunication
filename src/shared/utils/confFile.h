//
// Created by sylwia on 1/14/18.
//

#ifndef LINDACOMMUNICATION_CONFFILE_H
#define LINDACOMMUNICATION_CONFFILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ConfFile
{
private:
    std::ifstream * file;

public:
    ConfFile (const std::string & filename);
    ~ConfFile ();

    const std::string getProperty(const std::string & propertyName);

private:
    void setAtBeginning ();
};

#endif //LINDACOMMUNICATION_CONFFILE_H
