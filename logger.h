#ifndef LOGGER_H
#define LOGGER_H
#include<string>

class Logger
{
    static std::string directoryLocation;
public:
    Logger(std::string directoryLocation);
    static int printLog(std::string logMessage);
    static void changeDirectory(std::string newDirectoryLocation);
};

#endif // LOGGER_H
