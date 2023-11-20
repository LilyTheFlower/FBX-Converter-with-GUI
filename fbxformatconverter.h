#ifndef FBXFORMATCONVERTER_H
#define FBXFORMATCONVERTER_H
#include<string>
#include <fbxsdk.h>

class FBXFormatConverter
{
    enum class FileFormat{
        ascii,
        binary,
        unknown
    };

    struct FBXFile{
        FileFormat format;
        std::string* fileLocation;
    };
    FbxScene importFBX();
    int exportFBX();

public:
    FBXFormatConverter();
    int convertToAscii(FBXFormatConverter::FBXFile *file, bool deleteOriginal);
    int convertToBinary(FBXFormatConverter::FBXFile *file, bool deleteOriginal);
    bool isFBXFile(FBXFormatConverter::FBXFile *file);
};

#endif // FBXFORMATCONVERTER_H
