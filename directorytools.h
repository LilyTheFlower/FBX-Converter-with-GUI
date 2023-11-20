#ifndef DIRECTORYTOOLS_H
#define DIRECTORYTOOLS_H
#include<string>

class DirectoryTools
{
    std::string directoryLocation;
public:
    DirectoryTools(std::string newDirectoryLocation);
    int fileCount();
    void changeDirectory(std::string newDirectoryLocation);
    std::string* listFiles();
};

#endif // DIRECTORYTOOLS_H
