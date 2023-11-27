#ifndef LOGGER_H
#define LOGGER_H
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include<string>

class Logger
{
    std::string directoryLocation;
    std::string stringCurrentDateTime();
public:
    bool logging = true;
    Logger(std::string directoryLocation);
    int printLog(std::string logMessage);
    void changeDirectory(std::string newDirectoryLocation);
};

#endif // LOGGER_H
