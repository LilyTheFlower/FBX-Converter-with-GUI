#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <fbxformatconverter.h>
#include <QCloseEvent>
#include <QInputDialog>
#include "fileSaver.h"
#include <stdio.h>
std::string settingsFile = QDir::currentPath().toStdString().append("\\settings.txt");

std::string logFilePath = "";
std::string sourceFolderPath = "";
std::string destinationFolderPath = "";
bool ignoreFutureLogPrintFailures = false;

QCheckBox *cb;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //read the 3 pathes out of the settings and write them to the path variables as well as the ui
    logFilePath = readStringFromSettings("logPath");
    ui->logFolderLineEdit->setText(QString::fromStdString(logFilePath));
    sourceFolderPath = readStringFromSettings("sourceFolder");
    ui->sourceFolderLineEdit->setText(QString::fromStdString(sourceFolderPath));
    destinationFolderPath = readStringFromSettings("destinationFolder");
    ui->destinationFolderLineEdit->setText(QString::fromStdString(destinationFolderPath));

    //change log directory to match the one read from the settings file
    FBXFormatConverter::changeFBXLogDirectory(logFilePath);

    //read enableLog value from file and set the gui to match
    std::string enableLog = readStringFromSettings("enableLogging");
    if(enableLog.length() > 0){
        if(enableLog.compare("1") == 0){
            ui->enableLoggingCheckbox->setCheckState(Qt::Checked);
            FBXFormatConverter::enableFBXLogging(true);
        }else if(enableLog.compare("0") == 0){
            ui->enableLoggingCheckbox->setCheckState(Qt::Unchecked);
            FBXFormatConverter::enableFBXLogging(false);
        }
    }

    //read replaceOriginals value from file and set the gui to match
    std::string replaceOriginals = readStringFromSettings("replaceOriginals");
    if(replaceOriginals.size() > 0){
        if(replaceOriginals.compare("1") == 0){
            ui->replaceOriginalsCheckBox->setCheckState(Qt::Checked);
        }else if(replaceOriginals.compare("0") == 0){
            ui->replaceOriginalsCheckBox->setCheckState(Qt::Unchecked);
        }
    }

    //set table format settings
    ui->fbxFileTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->fbxFileTableWidget->setColumnWidth(0, ui->fbxFileTableWidget->width()*(0.666));
    ui->fbxFileTableWidget->setColumnWidth(1, ui->fbxFileTableWidget->width()*(0.333));
    ui->fbxFileTableWidget->verticalHeader()->setVisible(false);
    //fill table with files
    populateTable();

    //pass function to the FBX converter to call if the log print fails
    FBXFormatConverter::getNotifyUserFunction(onPrintLogFailure);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sourceFolderPushButton_clicked()
{
    //ask user to find folder
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose a Folder as the Source Directory", QDir::homePath());
    //put folder in line edit
    ui->sourceFolderLineEdit->setText(folderName);
    //save folder in variable
    sourceFolderPath = folderName.toStdString();
    populateTable();
}


void MainWindow::on_logFolderPushButton_clicked()
{
    //ask user to find folder
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose a Folder as the Log Directory", QDir::homePath());
    //put folder in line edit
    ui->logFolderLineEdit->setText(folderName);
    //save folder in variable
    logFilePath = folderName.toStdString();
    //change the log directory to the one the user chose
    FBXFormatConverter::changeFBXLogDirectory(logFilePath);
}

void MainWindow::on_destinationFolderPushButton_clicked()
{
    //ask user to find folder
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose a Folder as the Destination Directory", QDir::homePath());
    //put folder in line edit
    ui->destinationFolderLineEdit->setText(folderName);
    //save folder in variable
    destinationFolderPath = folderName.toStdString();
}


//Copy check box saving functionality for the other checkbox

void MainWindow::on_enableLoggingCheckbox_stateChanged(int arg1)
{
    char c;
    if(arg1){
        FBXFormatConverter::enableFBXLogging(true);
        c = '1';
        //see philosophy of return checking for saving settings
        //save this choice to the settings file
        fileSaver::save(settingsFile, &c, 1,  "enableLogging");
    }else{
        FBXFormatConverter::enableFBXLogging(false);
        c = '0';
        //see philosophy of return checking for saving settings
        //save this choice to the settings file
        fileSaver::save(settingsFile, &c, 1,  "enableLogging");
    }
}

void MainWindow::on_replaceOriginalsCheckBox_stateChanged(int arg1)
{
    char c;
    if(arg1){
        c = '1';
        //save this choice to the settings file
        fileSaver::save(settingsFile, &c, 1,  "replaceOriginals");
        //diable the destination folder line edit and button since the original files will be replaced
        ui->destinationFolderLineEdit->setDisabled(true);
        ui->destinationFolderPushButton->setDisabled(true);
    }else{
        c = '0';
        //save this choice to the settings file
        fileSaver::save(settingsFile, &c, 1,  "replaceOriginals");
        //enable the destination folder line edit and button since the original files will not be replaced
        ui->destinationFolderLineEdit->setDisabled(false);
        ui->destinationFolderPushButton->setDisabled(false);
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    //Philosophy on return checking for saving settings
    /*Im not checking the return value of these because they are being called as the program is closing
    Its possible that I could send a dialog explaining that settings couldnt be saved, but this could be a
    little annoying for the user. I would be far more concerned if the settings saving wasnt just quality
    of life and was more critical to the program's functionality*/

    //saving strings for the 3 different pathes selected by user to memory
    saveStringtoSettings("logPath", logFilePath);
    saveStringtoSettings("sourceFolder", sourceFolderPath);
    saveStringtoSettings("destinationFolder", destinationFolderPath);
    event->accept();
}

void MainWindow::saveStringtoSettings(std::string recordID, std::string data){
    //create a buffer of suffecient size to store pathes, I felt 200 char would suffice, but more maybe
    //necessary, I felt 200 was enough, but longer paths could theoretically happen. Might need to
    //switch to dynamic allocation
    char saveBuff[200];
    memset(saveBuff, 0, 200);
    strcat(saveBuff, data.c_str());
    //see philosophy on checking this return above
    fileSaver::save(settingsFile, saveBuff, data.length(),  recordID);
}

std::string MainWindow::readStringFromSettings(std::string recordID){
    //create empty record object pointer
    fileSaver::record* r;
    //read file and fill record object
    r = fileSaver::read(settingsFile, recordID);
    //check for reader success
    if(r->size > 0){
        return (char*)r->data;
    }else{
        return "";
    }
}

void MainWindow::populateTable(){
    //only operate if the path is not empty
    if(sourceFolderPath.compare("") != 0){
        //clear the table
        ui->fbxFileTableWidget->setRowCount(0);
        //create a QDir from the source folder path
        QDir folder(QString::fromStdString(sourceFolderPath));
        //find all files with the FBX extension
        QStringList filesList = folder.entryList(QStringList() << "*.fbx" << "*.FBX",QDir::Files);
        int i = 0;
        //iterate over all the files in the list
        foreach(QString file, filesList){
            //add a row to the table
            ui->fbxFileTableWidget->insertRow(i);
            //add the file name to this row of the table in the first column
            ui->fbxFileTableWidget->setItem(i, 0, new QTableWidgetItem(file));
            //calculate the full path to use checkFormat
            std::string fullPath;
            fullPath.append(sourceFolderPath);
            fullPath.append("/");
            fullPath.append(file.toStdString());
            //check the format of the file and set the type in the type column of the table
            FBXFormatConverter::FBXFormat fbxFormat = FBXFormatConverter::checkFormat(fullPath);
            if(fbxFormat == FBXFormatConverter::ascii){
                ui->fbxFileTableWidget->setItem(i, 1, new QTableWidgetItem("ascii"));
            }else if(fbxFormat == FBXFormatConverter::binary){
                ui->fbxFileTableWidget->setItem(i, 1, new QTableWidgetItem("binary"));
            }else{
                ui->fbxFileTableWidget->setItem(i, 1, new QTableWidgetItem("unknown"));
            }
            i++;
        }
    }
}


void MainWindow::on_selectPushButton_clicked()
{
    //clear the table selection
    ui->fbxFileTableWidget->clearSelection();
    //if the selection is none, just return, all have been unselected already
    if(ui->selectionTypeComboBox->currentText().compare("None") == 0){
        return;
    }
    //else we iterate over the table rows and select both columns of rows matching the selection combo box text
    for(int i = 0 ; i < ui->fbxFileTableWidget->rowCount(); i++){
        if(ui->selectionTypeComboBox->currentText().compare(ui->fbxFileTableWidget->item(i, 1)->text())== 0){
            ui->fbxFileTableWidget->item(i, 0)->setSelected(true);
            ui->fbxFileTableWidget->item(i, 1)->setSelected(true);
        }else if(ui->selectionTypeComboBox->currentText().compare("All") == 0){
            ui->fbxFileTableWidget->item(i, 0)->setSelected(true);
            ui->fbxFileTableWidget->item(i, 1)->setSelected(true);
        }
    }
}


void MainWindow::on_fbxFileTableWidget_itemSelectionChanged()
{
    //if one side of a table row is selected, select the other side too
    for(int i = 0 ; i < ui->fbxFileTableWidget->rowCount(); i++){
        if(ui->fbxFileTableWidget->item(i, 0)->isSelected() && !ui->fbxFileTableWidget->item(i, 1)->isSelected()){
            ui->fbxFileTableWidget->item(i, 1)->setSelected(true);
        }else if(ui->fbxFileTableWidget->item(i, 1)->isSelected() && !ui->fbxFileTableWidget->item(i, 0)->isSelected()){
            ui->fbxFileTableWidget->item(i, 0)->setSelected(true);
        }
    }
}


void MainWindow::on_conversionPushButton_clicked()
{
    //create the conversion error message box
    QMessageBox conversionMsgbox;
    bool ignoreFutureConversionFailures = false;
    QCheckBox *ccb = new QCheckBox("Do not show again");
    conversionMsgbox.setIcon(QMessageBox::Icon::Warning);
    conversionMsgbox.addButton(QMessageBox::Ok);
    conversionMsgbox.setDefaultButton(QMessageBox::Ok);
    conversionMsgbox.setCheckBox(ccb);
    float count = 0;
    //iterate over all files in the table
    for(int i = 0 ; i < ui->fbxFileTableWidget->rowCount(); i++){
        //only operate on selected files
        if(ui->fbxFileTableWidget->item(i, 0)->isSelected()){
            //create full in path
            std::string fullInPath;
            fullInPath.append(sourceFolderPath);
            fullInPath.append("/");
            fullInPath.append(ui->fbxFileTableWidget->item(i, 0)->text().toStdString());

            //create full out path
            std::string fullOutPath;
            fullOutPath.append(destinationFolderPath);
            fullOutPath.append("/");

            FBXFormatConverter::FBXFormat format =  FBXFormatConverter::unknown;

            //not replacing original, rename new file something different indicating conversion
            if(ui->conversionTypeComboBox->currentText() == "ascii"){
                //append a "binary_" to the front of the file name
                format = FBXFormatConverter::ascii;
                fullOutPath.append("ascii_");
                fullOutPath.append(ui->fbxFileTableWidget->item(i, 0)->text().toStdString());
            }else {
                //append a "binary_" to the front of the file name
                format = FBXFormatConverter::binary;
                fullOutPath.append("binary_");
                fullOutPath.append(ui->fbxFileTableWidget->item(i, 0)->text().toStdString());
            }

            //if replace originals is selected, out path and in path are the same
            if(ui->replaceOriginalsCheckBox->isChecked()){
                fullOutPath = fullInPath;
            }
            //convert file and check return value to know if we should update progress bar
            if(FBXFormatConverter::convertFile(fullInPath, fullOutPath, true, format)){
                //increment counter of complted files
                count++;
                //set progress bar update
                ui->conversionProgressBar->setValue((count/(float)(ui->fbxFileTableWidget->selectedItems().count()/2))*100);
            }else{
                //check to see if the user has denied seeing conversion error dialogs during this conversion run
                if(!ignoreFutureConversionFailures){
                    //create text unique to this failure
                    QString text = "Failed to convert ";
                    text .append(ui->fbxFileTableWidget->item(i, 0)->text().toStdString());
                    conversionMsgbox.setText(text);
                    //show dialog
                    conversionMsgbox.exec();
                    //check state of checkbox to know if showing more dialogs is okay
                    ignoreFutureConversionFailures = ccb->checkState();
                }
            }
        }
    }
    populateTable();
}

//this function gets passed to the fbx converter class to be called when there is an issue with the log printing
void MainWindow::onPrintLogFailure(){
    if(!ignoreFutureLogPrintFailures){
        QMessageBox msgbox;
        cb = new QCheckBox("Do not show again");
        //setup print log failure dialog
        msgbox.setText("Printing to the Log Failed");
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.addButton(QMessageBox::Ok);
        msgbox.setDefaultButton(QMessageBox::Ok);
        msgbox.setCheckBox(cb);
        //make a dialog happen when print to log fails and see if user wants to be notified about them more times this run of the program
        msgbox.exec();
        //check state of checkbox to know if showing more dialogs is okay
        ignoreFutureLogPrintFailures = cb->checkState();
    }
}

