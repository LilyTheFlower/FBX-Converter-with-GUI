#include "filesaver.h"
#include <iostream>
#include <stdio.h>
fileSaver::fileSaver()
{

}

int fileSaver::save(std::string file, void* data, int sizeOfData, std::string recordID){
    FILE* saveFile = fopen(file.c_str(), "a+");
    if(!saveFile){
        printf("failed to open the file, notify the user via gui\n");
        return -1;
    }
    //create the header string
    //recordname_dataLength_Data
    //underscore is the delimiter, dataLength is the length of the data, and then the data begins after another delimiter
    std::string sizeString = std::to_string(sizeOfData);
    std::string writeRecordID;
    std::string writeRecordSize;
    writeRecordID.append(recordID);
    writeRecordID += delimiter;
    writeRecordSize.append(sizeString);
    writeRecordSize += delimiter;

    fseek(saveFile, 0, SEEK_END);
    int totalBytes = ftell(saveFile);
    fseek(saveFile, 0, SEEK_SET);
    int search = fileSaver::findRecord(saveFile, recordID);

    if(search == totalBytes){
        //didn't find the record, free to put the new record at the end of the file
        //put in the header string as the first part of the string to be added to the file
        std::fputs(writeRecordID.c_str(), saveFile);
        //put in the header string as the first part of the string to be added to the file
        std::fputs(writeRecordSize.c_str(), saveFile);
        //then turn data into chars and place in file
        for(int i = 0; i < sizeOfData; i++){
            char c = *((char*)data + i);
            fputc(c, saveFile);
        }
        fclose(saveFile);
        return 1;
    }else if(search > -1){
        //record found, location is equal to where the size of the record begins

        //prepare to store the data from before record and after it
        std::string before;
        std::string after;
        while(ftell(saveFile) + 1 != search - recordID.length()){
            before += fgetc(saveFile);
        }
        //now skip over the record that is being updated

        //move to the spot to read the size of the old record data
        fseek(saveFile, search, SEEK_SET);
        std::string readSize;
        //read the size of the record
        char c = 'a';
        do{
            c = fgetc(saveFile);
            if(c == -1){
                //error occured while reading since we know the record exists already
                printf("failed to read file, notify user via gui, send junk record\n");
                return -1;
            }
            if(c!= delimiter){
                readSize += c;
            }
        }while(c!= delimiter);
        fseek(saveFile, stoi(readSize), SEEK_CUR);
        while((c = fgetc(saveFile)) != -1){
            after +=c;
        }
        fclose(saveFile);
        FILE* newFile = fopen(file.c_str(), "w+");
        if(!saveFile){
            printf("failed to re-open the file, notify the user via gui\n");
            return -1;
        }
        fputs(before.c_str(), newFile);
        //put in the header string as the first part of the string to be added to the file
        std::fputs(writeRecordID.c_str(), newFile);
        //put in the header string as the first part of the string to be added to the file
        std::fputs(writeRecordSize.c_str(), newFile);
        //then turn data into chars and place in file
        for(int i = 0; i < sizeOfData; i++){
            char c = *((char*)data + i);
            fputc(c, newFile);
        }
        fputs(after.c_str(), newFile);
        fclose(newFile);
        return 1;
    }else{
        printf("error occured in searching the file for an existing record while saving the record");
        fclose(saveFile);
        return -1;
    }
    printf("Should never reach this place in save, something went really weird");
    fclose(saveFile);
    return -1;
}

fileSaver::record* fileSaver::read(std::string file, std::string recordID){
    //create record and fill it with junk for the time being
    fileSaver::record* r = new record();
    r->size = -1;

    FILE* saveFile = fopen(file.c_str(), "a+");
    if(!saveFile){
        printf("failed to open the file while reading for a record, notify the user via gui, send junk record\n");
        return r;
    }

    fseek(saveFile, 0, SEEK_END);
    int totalBytes = ftell(saveFile);
    fseek(saveFile, 0, SEEK_SET);
    int search = fileSaver::findRecord(saveFile, recordID);

    if(search == totalBytes){
        //reached the end of the file and record not found
        return r;
    }else if(search > -1){
        //record found, location is equal to where the size of the record begins
        fseek(saveFile, search, SEEK_SET);
        //the record is found, and the FILE* stream is pointing to the beginning of the record
        char c = 'a';
        std::string readRecordID;
        std::string readSize;
        //read the size of the record
        do{
            c = fgetc(saveFile);
            if(c == -1){
                //error occured while reading since we know the record exists already
                printf("failed to read file while trying to read record size, notify user via gui, send junk record\n");
                return r;
            }
            if(c!= delimiter){
                readSize += c;
            }
        }while(c!= delimiter);
        //create int of the size of the record
        int dataSize = stoi(readSize);
        //prepare a pointer for the raw data being read
        char* data = (char*)malloc(dataSize);
        //read the data byte by byte into memory
        for(int i = 0; i <dataSize; i++){
            //read the byte out of the file
            c = fgetc(saveFile);
            if(c == -1){
                //error occured while reading since we know the record exists already
                printf("failed to read file while trying to read record data, notify user via gui, send junk record\n");
                return r;
            }
            //put the byte of data into memory
            data[i] = c;
        }
        //put the data and size of the record into a struct
        r->data = data;
        r->size = dataSize;
        //close the file
        fclose(saveFile);
        //return the struct containing the data
        return r;
    }else{
        //error occured looking for the record
        printf("error occured while trying to read record\n");
        return r;
    }
    printf("Should never reach this place in read, something went really weird\n");
    fclose(saveFile);
    return r;
}

int fileSaver::findRecord(FILE* saveFile, std::string recordID){
    //save the stream's place
    int placeHolder = ftell(saveFile);
    //record the total length of the file
    fseek(saveFile, 0, SEEK_END);
    int totalBytes = ftell(saveFile);
    //begin at first record, move through header, read ID, store, then read size
    fseek(saveFile, 0, SEEK_SET);
    char c = 'a';
    //begin looping through records, stopping if the read character is EOF or -1 from error
    while(c != -1){
        //prepare variables for the record ID and the readSize, prepare new ones each time the loop goes around
        std::string readRecordID;
        std::string readSize;
        //begin reading the record ID
        do{
            //get character from file
            c = fgetc(saveFile);
            if(c == EOF){
                printf("reached end of file, head to error checking\n");
                break;
            }
            if(c!= delimiter){
                //character wasn't delimiter or -1, so it is part of record ID
                readRecordID += c;
            }
            //end loop when delimiter is reached
        }while(c!= delimiter);
        //now start reading the size of the record
        do{
            //get character from file
            c = fgetc(saveFile);
            if(c == -1){
                printf("reached end of file, head to error checking\n");
                break;
            }
            if(c!= delimiter){
                //character wasn't delimiter or -1, so it is part of record size
                readSize += c;
            }
        }while(c!= delimiter);
        //check to see if the record ID matches the one being searched for
        if(readRecordID.compare(recordID.c_str()) == 0){
            //record ID does match, record found! record the location of where to start reading to get this size for this record
            int search = ftell(saveFile)-(readSize.size()+1);
            //reset the file to where it was before searching it
            fseek(saveFile, placeHolder, SEEK_SET);
            //return the recorded location
            return search;
        }else if(c != -1){
            //did not find the record, skip ahead by the size of the record data and try again on the next record
            fseek(saveFile, stoi(readSize), SEEK_CUR);
        }
    }
    if(ftell(saveFile) == totalBytes){
        //reached the end of the file successfully, and the record was not found

        //reset the location of the file pointer
        fseek(saveFile, placeHolder, SEEK_SET);
        //the place to put a new record is at the end of the file
        return totalBytes;
    }else{
        printf("there was most likely an error, try to reset the file to its position and return -1\n");
        fseek(saveFile, placeHolder, SEEK_SET);
        return -1;
    }
    printf("Should never reach this place in find record, something went really weird\n");
    fseek(saveFile, placeHolder, SEEK_SET);
    return -1;
}

int fileSaver::findRecord(std::string file, std::string recordID){
    FILE* saveFile = fopen(file.c_str(), "a+");
    if(!saveFile){
        printf("failed to open the file, notify the user via gui, send junk record\n");
        return -1;
    }
    int search = fileSaver::findRecord(saveFile, recordID);
    fclose(saveFile);
    return search;
}
