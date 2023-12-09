#ifndef FILESAVER_H
#define FILESAVER_H
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include<string>

static char delimiter = '_';
class fileSaver
{
public:
    //CAUTION!!! be aware that the record ID cannot contain the "_" delimiter character as the funciton is written, to change this, choose another delimiter character that wont be in your record ID
    struct record{
        void* data;
        int size;
    };
    fileSaver();
    static bool save(std::string file, void* data, int sizeOfData, std::string recordID);
    static record* read(std::string file, std::string recordID);

    static int findRecord(FILE* saveFile, std::string recordID);
    static int findRecord(std::string file, std::string recordID);
};

#endif // FILESAVER_H
