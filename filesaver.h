#ifndef FILESAVER_H
#define FILESAVER_H
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include<string>

class fileSaver
{
public:
    //CAUTION!!! be aware that the record ID cannot contain the "_" delimiter character as the funciton is written, to change this, choose another delimiter character that wont be in your record ID
    char delimiter = '_';
    struct record{
        void* data;
        int size;
    };
    fileSaver();
    int save(std::string file, void* data, int sizeOfData, std::string recordID);
    record* read(std::string file, std::string recordID);
};

#endif // FILESAVER_H
