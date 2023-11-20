#include "directorytools.h"
#include "Logger.h"
#include <QDir.h>

DirectoryTools::DirectoryTools(std::string newDirectoryLocation)
{
    //check to see if the directoy exists
    if(QDir(newDirectoryLocation.c_str()).exists()){
        //the directory exists, print message to log and set the directory location to the one supplied in this constructor
        Logger::printLog("Directory found successfully, accessing");
        directoryLocation = newDirectoryLocation;
    }else{
        Logger::printLog("Directory invalid or inaccessible");
        //handle the directory being missing, notify the user though gui in some way
    }

}

int DirectoryTools::fileCount(){
    directoryLocation;
}

void changeDirectory(std::string newDirectoryLocation){

}

std::string* listFiles(){

}
