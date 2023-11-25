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
    FBXFormatConverter();
    static int convertFile(std::string sourceLocation, std::string destinationLocaiton, bool deleteOriginal, std::string format);
    static int isFBXFile(std::string sourceLocation);
    static std::string checkFormat();
};

#endif // FBXFORMATCONVERTER_H
