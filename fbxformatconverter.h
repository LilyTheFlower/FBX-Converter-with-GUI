#ifndef FBXFORMATCONVERTER_H
#define FBXFORMATCONVERTER_H
#include<string>
#include <fbxsdk.h>
#include <logger.h>
#include <functional>
class FBXFormatConverter
{
    struct FBXFile{
        //format options are "ascii" "binary" and "unknown"
        std::string format;
        std::string* fileLocation;
    };
public:
    enum FBXFormat{
        ascii, binary, unknown
    };

    FBXFormatConverter();
    static bool convertFile(std::string sourceLocation, std::string destinationLocaiton, bool deleteOriginal, FBXFormat format);
    static int isFBXFile(std::string sourceLocation);
    static FBXFormat checkFormat(std::string sourceLocation);
    static void enableFBXLogging(bool enable);
    static bool changeFBXLogDirectory(std::string newDirectory);
    static void getNotifyUserFunction(std::function<void()> notifyUser);
private:
    static FbxScene* importFBX(std::string sourceLocation);
    static bool exportFBX(std::string destinationLocaiton, FbxScene* scene, FBXFormat format);

};

#endif // FBXFORMATCONVERTER_H
