#include "fbxformatconverter.h"
#include <fbxsdk.h>
#include <logger.h>


Logger* FBXLog;
FbxManager* lSdkManager;
FbxScene* nullScene;

//default values, reconfigured later
int binaryID;
int asciiID;
int binary6ID;
int ascii6ID;
int encryptedID;
int encrypted6ID;

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
            if(strcmp(formatDesctiptions, "FBX binary (*.fbx)") == 0)
            {
                binaryID = i;
            }
            else if(strcmp(formatDesctiptions, "FBX ascii (*.fbx)") == 0)
            {
                asciiID = i;
            }else if(strcmp(formatDesctiptions, "FBX 6.0 binary (*.fbx)") == 0){
                binary6ID = i;
            }else if(strcmp(formatDesctiptions, "FBX 6.0 ascii (*.fbx)") == 0){
                ascii6ID = i;
            }else if(strcmp(formatDesctiptions, "FBX encrypted (*.fbx)") == 0){
                encryptedID = i;
            }else if(strcmp(formatDesctiptions, "FBX 6.0 encrypted (*.fbx)") == 0){
                encrypted6ID = i;
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

bool FBXFormatConverter::exportFBX(std::string destinationLocaiton, FbxScene* scene, std::string format){
    // Create an IOSettings object.
    FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
    lSdkManager->SetIOSettings(ios);

    // ... Configure the FbxIOSettings object here ...

    // Create an exporter.
    FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");

    // Declare the path and filename of the file to which the scene will be exported.
    // In this case, the file will be in the same directory as the executable.
    const char* lFilename = "file.fbx";
    const int fileFormat = -1;
    if(format.compare("binary") == 0){

    //determine the intended destination format for the file depending on the user input
    }else if(format.compare("ascii") == 0){
    }else{
        FBXLog->printLog("unknown file format provided");
        return false;
    }
    // Initialize the exporter.
    bool lExportStatus = lExporter->Initialize(lFilename, fileFormat, lSdkManager->GetIOSettings());

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


int FBXFormatConverter::convertFile(std::string sourceLocation, std::string destinationLocaiton, bool deleteOriginal, std::string format){
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
        msg.append(format);
        //print the message to the log
        FBXLog->printLog(msg);
        return -1;
    }

    if(exportFBX(destinationLocaiton, importedScene, format)){
        //the export was successful
        std::string msg = "Exported ";
        msg.append(format);
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
        msg.append(format);
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
    pIOPluginRegistry->DetectReaderFileFormat(sourceLocation.c_str(), formatID);
    if(pIOPluginRegistry->ReaderIsFBX( formatID )){
        FBXLog->printLog("FBX file verified");
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
    int isfbx = isFBXFile(sourceLocation);
    if(isfbx != -1){
        //The file is an fbx
        if(isfbx == binaryID){
            return FBXFormatConverter::binary;
        }else if(isfbx == asciiID){
            return FBXFormatConverter::ascii;
        }else if(isfbx == encryptedID){
            return FBXFormatConverter::encrypted;
        }else if(isfbx == binary6ID){
            return FBXFormatConverter::binary6;
        }else if(isfbx == encrypted6ID){
            return FBXFormatConverter::encrypted6;
        }else if(isfbx == ascii6ID){
            return FBXFormatConverter::ascii6;
        }
        return FBXFormatConverter::unknown;
    }else{
        return FBXFormatConverter::unknown;
    }
}

void FBXFormatConverter::enableFBXLogging(bool enable){
    FBXLog->enableLogging(enable);
}

void FBXFormatConverter::changeFBXLogDirectory(std::string newDirectory){
    FBXLog->changeDirectory(newDirectory);
}
