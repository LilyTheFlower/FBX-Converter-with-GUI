#include "logger.h"
#include <qdir.h>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <stdio.h>
#include <time.h>
Logger::Logger(std::string directoryLocation){
    changeDirectory(directoryLocation);
}

bool Logger::printLog(std::string logMessage){
    if(!logging){
        return true;
    }
    FILE* logFile = fopen(directoryLocation.c_str(), "a+");
    if(!logFile){
        return false;
    }
    //print to the log file
    if(logFile){
        std::string msg = stringCurrentDateTime(true);
        msg.append(": ");
        msg.append(logMessage);
        msg.append("\n");
        fputs(msg.c_str(), logFile);
        fclose(logFile);
        return true;
    }
    return false;
}

bool Logger::changeDirectory(std::string newDirectoryLocation){
    newDirectoryLocation.append("\\");
    std::string timeFileName = stringCurrentDateTime(false);
    timeFileName.append(".txt");
    newDirectoryLocation.append(timeFileName);
    directoryLocation = newDirectoryLocation;

    //check to see if the directory is valid
    FILE* f = fopen(directoryLocation.c_str(), "a+");
    if(f){
        fclose(f);
        return true;
    }else{
        return false;
    }
}

// Get current date/time, format is YYYY-MM-DD-HH-mm
std::string Logger::stringCurrentDateTime(bool includeSeconds) {
    char buf[80];
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);
    if(includeSeconds){
        strftime(buf, sizeof(buf), "%Y-%m-%d-%X", &tstruct);
    }else{
        strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M", &tstruct);
    }
    return buf;
}

void Logger::enableLogging(bool enabled){
    if(!logging && enabled){
        logging = enabled;
        printLog("Logging Enabled");
    }else if(logging && !enabled){
        printLog("Logging Disabled");
        logging = enabled;
    }
}
