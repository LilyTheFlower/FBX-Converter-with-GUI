#ifndef LOGGER_H
#define LOGGER_H
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include<string>

class Logger
{
    std::string directoryLocation;
    std::string stringCurrentDateTime(bool includeSeconds);
    bool logging = false;
public:
    Logger(std::string directoryLocation);
    int printLog(std::string logMessage);
    void changeDirectory(std::string newDirectoryLocation);
    void enableLogging(bool enabled);
};

#endif // LOGGER_H
