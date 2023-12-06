#ifndef FBXFORMATCONVERTER_H
#define FBXFORMATCONVERTER_H
#include<string>
#include <fbxsdk.h>
#include <logger.h>

class FBXFormatConverter
{
    struct FBXFile{
        //format options are "ascii" "binary" and "unknown"
        std::string format;
        std::string* fileLocation;
    };



    static FbxScene* importFBX(std::string sourceLocation);
    static bool exportFBX(std::string destinationLocaiton, FbxScene* scene, std::string format);

public:
    enum FBXFormat{
        ascii, binary, ascii6, binary6, encrypted, encrypted6, unknown
    };
    FBXFormatConverter();
    static int convertFile(std::string sourceLocation, std::string destinationLocaiton, bool deleteOriginal, std::string format);
    static int isFBXFile(std::string sourceLocation);
    static FBXFormat checkFormat(std::string sourceLocation);
    static void enableFBXLogging(bool enable);
    static void changeFBXLogDirectory(std::string newDirectory);
};

#endif // FBXFORMATCONVERTER_H
