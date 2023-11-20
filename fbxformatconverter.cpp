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
            if ( strcmp(formatDesctiptions, "binary") == 0 )
            {
                const_cast<int&>(binaryID) = i;
            }
            else if ( strcmp(formatDesctiptions, "ascii") == 0 )
            {
                const_cast<int&>(asciiID) = i;
            }
        }
    }
}


FbxScene* FBXFormatConverter::importFBX(std::string sourceLocation){

    //Following Code comes straight from the FBX SDK documentation, edited only to target the file at the address provided by fileLocation:


    // Create an IOSettings object.
    FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
    lSdkManager->SetIOSettings(ios);

    // ... Configure the FbxIOSettings object ...

    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    // Declare the path and filename of the file containing the scene.
    // In this case, we are assuming the file is in the same directory as the executable.
    const char* lFilename = sourceLocation.c_str();

    // Initialize the importer.
    bool lImportStatus = lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings());

    //End Documentation Code

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

    }else if(format.compare("ascii") == 0){

    }else{
        FBXLog->printLog("unknown file format provided");
        return false;
    }
    // Initialize the exporter.
    bool lExportStatus = lExporter->Initialize(lFilename, fileFormat, lSdkManager->GetIOSettings());

    if(!lExportStatus) {
        printf("Call to FbxExporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
        return false;
    }

    lExporter->Export(scene);
    // Destroy the exporter.
    lExporter->Destroy();
    return true;
}


int FBXFormatConverter::convertFile(std::string sourceLocation, std::string destinationLocaiton, bool deleteOriginal, std::string format){
    FbxScene* importedScene = importFBX(sourceLocation);
    if(importedScene == nullScene){
        //this means that an error occured in the import process, and the return is equal to the dummy nullScene as a result
        std::string msg = "";
        msg.append("FAILED TO IMPORT! Source Location: ");
        msg.append(sourceLocation);
        msg.append("Format requested: ");
        msg.append(format);
        FBXLog->printLog(msg);
        return -1;
    }

    if(exportFBX(destinationLocaiton, importedScene, format)){
        std::string msg = "Exported ";
        msg.append(format);
        msg.append(" to: ");
        msg.append("destinationLocation");
        FBXLog->printLog(msg);
    }else{
        std::string msg = "";
        msg.append("FAILED TO EXPORT! Source Location: ");
        msg.append(sourceLocation);
        msg.append("Format requested: ");
        msg.append(format);
        FBXLog->printLog(msg);
        return -1;
    }
    return 0;
}

bool FBXFormatConverter::isFBXFile(FBXFormatConverter::FBXFile *file){
    return true;
}
