#include "fbxformatconverter.h"
#include <fbxsdk.h>
#include <logger.h>


Logger* FBXLog;
FbxManager* lSdkManager;
FbxScene* nullScene;

//default values, reconfigured later
int binaryID;
int asciiID;

FBXFormatConverter::FBXFormatConverter(){
    FBXLog = new Logger("\\logs");
    // Create the FBX SDK manager
    lSdkManager = FbxManager::Create();
    //dummy scene to allow recognition of when an error has occured while still allowing for the same return type and no new error tracking classes or external objects
    nullScene = FbxScene::Create(lSdkManager,"nullScene");

    //default the values
    binaryID = -1;
    asciiID = -1;

    // Find the IDs for the ascii and binary writers
    int const formatCount = lSdkManager->GetIOPluginRegistry()->GetWriterFormatCount();
    for ( int i = 0; i < formatCount; i++ )
    {
        if (lSdkManager->GetIOPluginRegistry()->WriterIsFBX( i ) )
        {
            char const* formatDesctiptions = lSdkManager->GetIOPluginRegistry()->GetWriterFormatDescription( i );
            printf("%s\n", formatDesctiptions);
            if(strcmp(formatDesctiptions, "FBX binary (*.fbx)") == 0){
                binaryID = i;
            }
            else if(strcmp(formatDesctiptions, "FBX ascii (*.fbx)") == 0){
                asciiID = i;
            }
        }
    }

}


FbxScene* FBXFormatConverter::importFBX(std::string sourceLocation){
    // Create an IOSettings object.
    FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
    lSdkManager->SetIOSettings(ios);

    // ... Configure the FbxIOSettings object ...

    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Declare the path and filename of the file containing the scene.
    // In this case, we are assuming the file is in the same directory as the executable.
    const char* lFilename = sourceLocation.c_str();

    if(FBXFormatConverter::isFBXFile(sourceLocation) == -1){
        //the file is not an fbx or the type could not be identified, method has already updated log
        return nullScene;
    }

    // Initialize the importer.
    bool lImportStatus = lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings());

    if(!lImportStatus) {
        FBXLog->printLog("Call to FbxImporter::Initialize() failed.\n");
        char* error = new char[200];
        sprintf(error, "Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        FBXLog->printLog(error);
        return nullScene;
    }
    // Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager,"conversionScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);
    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();
    return lScene;
}

bool FBXFormatConverter::exportFBX(std::string destinationLocaiton, FbxScene* scene, FBXFormat format){
    // Create an IOSettings object.
    FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
    lSdkManager->SetIOSettings(ios);

    // ... Configure the FbxIOSettings object here ...

    // Create an exporter.
    FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");

    // Declare the path and filename of the file to which the scene will be exported.
    // In this case, the file will be in the same directory as the executable.
    int fileFormat = -1;
    if(format == FBXFormatConverter::ascii){
        fileFormat = asciiID;
    }else if(format == FBXFormatConverter::binary){
        fileFormat = binaryID;
    }else{
        FBXLog->printLog("unknown file format provided");
        return false;
    }
    // Initialize the exporter.
    bool lExportStatus = lExporter->Initialize(destinationLocaiton.c_str(), fileFormat, lSdkManager->GetIOSettings());

    //get the exporter status
    if(!lExportStatus) {

        FBXLog->printLog("Call to FbxExporter::Initialize() failed.\n");
        char* error = new char[200];
        sprintf(error, "Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
        FBXLog->printLog(error);
        return false;
    }

    // export the scene
    lExporter->Export(scene);
    // Destroy the exporter.
    lExporter->Destroy();
    return true;
}


int FBXFormatConverter::convertFile(std::string sourceLocation, std::string destinationLocaiton, bool deleteOriginal, FBXFormat format){
    FbxScene* importedScene = importFBX(sourceLocation);
    if(importedScene != nullScene){
        //the import was successfull

        //build string for the log
        std::string msg = "Imported ";
        msg.append(" file from: ");
        msg.append(sourceLocation);
        //print the message to the log
        FBXLog->printLog(msg);
    }else{
        //this means that an error occured in the import process, and the return is equal to the dummy nullScene as a result

        //build string for the log
        std::string msg = "";
        msg.append("FAILED TO IMPORT! Source Location: ");
        msg.append(sourceLocation);
        msg.append(" Format requested: ");
        if(format == FBXFormatConverter::ascii){
            msg.append("ascii");
        }else if(format == FBXFormatConverter::binary){
            msg.append("binary");
        }else{
            msg.append("unknown");
        }
        //print the message to the log
        FBXLog->printLog(msg);
        return -1;
    }

    if(exportFBX(destinationLocaiton, importedScene, format)){
        //the export was successful
        std::string msg = "Exported ";
        if(format == FBXFormatConverter::ascii){
            msg.append("ascii");
        }else if(format == FBXFormatConverter::binary){
            msg.append("binary");
        }else{
            msg.append("unknown");
        }
        msg.append(" to: ");
        msg.append(destinationLocaiton);
        FBXLog->printLog(msg);
    }else{
        //there was an error exporting

        //build string for the log
        std::string msg = "";
        msg.append("FAILED TO EXPORT! Source Location: ");
        msg.append(sourceLocation);
        msg.append(" Format requested: ");
        if(format == FBXFormatConverter::ascii){
            msg.append("ascii");
        }else if(format == FBXFormatConverter::binary){
            msg.append("binary");
        }else{
            msg.append("unknown");
        }
        //print the message to the log
        FBXLog->printLog(msg);
        return -1;
    }
    return 0;
}


// this function returns a number greater than -1 the corresponds to the format ID of the file
// if this function returns -1 the file was not identifiable.
int FBXFormatConverter::isFBXFile(std::string sourceLocation){
    int formatID = -1;
    auto pIOPluginRegistry = lSdkManager->GetIOPluginRegistry();
    pIOPluginRegistry->DetectWriterFileFormat(sourceLocation.c_str(), formatID);
    if(pIOPluginRegistry->ReaderIsFBX( formatID )){
        std::string msg = "";
        msg.append("Verified file at: ");
        msg.append(sourceLocation);
        msg.append(" as FBX");
        FBXLog->printLog(msg);
        return formatID;
    }
    std::string msg = "";
    msg.append("FAILED TO DETECT FILE FORMAT. Source Location:  ");
    msg.append(sourceLocation);
    msg.append(" Aborting import process...");
    FBXLog->printLog(msg);
    return -1;
}

FBXFormatConverter::FBXFormat FBXFormatConverter::checkFormat(std::string sourceLocation){
    std::string msg = "";
    int isfbx = isFBXFile(sourceLocation);
    if(isfbx != -1){
        FILE* f = fopen(sourceLocation.c_str(), "r");
        char c;
        do{
            c = fgetc(f);
            if(c == 0 || c > 127){
                fclose(f);
                msg.append("Identified file at: ");
                msg.append(sourceLocation);
                msg.append(" as binary FBX");
                FBXLog->printLog(msg);
                return FBXFormatConverter::binary;
            }
        }while(c!= EOF);
        fclose(f);
        msg.append("Identified file at: ");
        msg.append(sourceLocation);
        msg.append(" as ascii FBX");
        FBXLog->printLog(msg);
        return FBXFormatConverter::ascii;
    }else{
        msg.append("UNKNOWN FBX FILE AT: ");
        msg.append(sourceLocation);
        FBXLog->printLog(msg);
        return FBXFormatConverter::unknown;
    }
}

void FBXFormatConverter::enableFBXLogging(bool enable){
    FBXLog->enableLogging(enable);
}

void FBXFormatConverter::changeFBXLogDirectory(std::string newDirectory){
    FBXLog->changeDirectory(newDirectory);
}
