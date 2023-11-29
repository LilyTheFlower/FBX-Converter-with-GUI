#include "filesaver.h"
#include <iostream>
#include <stdio.h>
fileSaver::fileSaver()
{

}

int fileSaver::save(std::string file, void* data, int sizeOfData, std::string recordID){
    FILE* saveFile = fopen(file.c_str(), "a+");
    if(!saveFile){
        //failed to open the file, notify the user via gui
        return -1;
    }
    //create the header string
    //recordname_dataLength_Data
    //underscore is the delimiter, dataLength is the length of the data, and then the data begins after another delimiter
    std::string sizeString = std::to_string(sizeOfData);
    std::string header;
    header.append(recordID);
    header += delimiter;
    header.append(sizeString);
    header += delimiter;

    //put in the header string as the first part of the string to be added to the file
    std::fputs(header.c_str(), saveFile);

    //then turn data into chars and place in file
    for(int i = 0; i < sizeOfData; i++){
        char c = *((char*)data + i);
        fputc(c, saveFile);
    }
    fclose(saveFile);
}

fileSaver::record* fileSaver::read(std::string file, std::string recordID){
    //create record and fill it with junk for the time being
    fileSaver::record* r;
    r->size = -1;

    FILE* saveFile = fopen(file.c_str(), "a+");
    if(!saveFile){
        //failed to open the file, notify the user via gui, send junk record
        return r;
    }
    //begin at first record, move through header, read ID, store, then read size
    char c = 'a';
    while(c != -1){
        std::string readRecordID;
        std::string readSize;
        do{
            c = fgetc(saveFile);
            if(c == -1){
                //failed to read file, notify user via gui, send junk record
                return r;
            }
            if(c!= delimiter){
               readRecordID += c;
            }
        }while(c!= delimiter);
        do{
            c = fgetc(saveFile);
            if(c == -1){
               //failed to read file, notify user via gui, send junk record
               return r;
            }
            if(c!= delimiter){
               readSize += c;
            }
        }while(c!= delimiter);
        if(readRecordID.compare(recordID) == 0){
            //found the record, read data and return
            int dataSize = stoi(readSize);
            char* data = (char*)malloc(dataSize);
            for(int i = 0; i <dataSize; i++){
               c = fgetc(saveFile);
               if(c == -1){
                   //failed to read file, notify user via gui, send junk record
                   return r;
               }
               data[i] = c;
            }
            r->size = dataSize;
            r->data = data;
            return r;
        }else{
            //did not find the record, skip ahead this amount and try again
            fseek(saveFile, stoi(readSize), SEEK_CUR);
        }
    }
    //reached end of file and did not find the record, notify user via gui, send junk record
    return r;
}
