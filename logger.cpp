#include "logger.h"
#include <qdir.h>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
Logger::Logger(std::string directoryLocation){
    changeDirectory(directoryLocation);
}

int Logger::printLog(std::string logMessage){
    if(!logging){
        return 0;
    }
    FILE* logFile = fopen(directoryLocation.c_str(), "w+");
    if(!logFile){
        //failed to open the file, notify the user via gui
        return -1;
    }
    //print to the log file
    if(logFile){
        std::string msg = stringCurrentDateTime();
        msg.append(": ");
        msg.append(logMessage);
        msg.append("\n");
        fputs(msg.c_str(), logFile);
        fclose(logFile);
        return 0;
    }
    return -1;
}

void Logger::changeDirectory(std::string newDirectoryLocation){
    newDirectoryLocation.append("\\");
    std::string timeFileName = stringCurrentDateTime();
    timeFileName.append(".txt");
    newDirectoryLocation.append(timeFileName);
    directoryLocation = newDirectoryLocation;
}

// Get current date/time, format is YYYY-MM-DD-HH-mm
std::string Logger::stringCurrentDateTime() {
    char buf[80];
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M", &tstruct);

    return buf;
}
