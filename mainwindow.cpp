#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <fbxformatconverter.h>
#include <QCloseEvent>
#include "fileSaver.h"
#include <stdio.h>
std::string settingsFile = QDir::currentPath().toStdString().append("\\settings.txt");

std::string logFilePath = "";
std::string sourceFolderPath = "";
std::string destinationFolderPath = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileSaver::record* r;
    r = fileSaver::read(settingsFile, "logPath");

    logFilePath = readStringFromSettings("logPath");
    ui->logFolderLineEdit->setText(QString::fromStdString(logFilePath));
    sourceFolderPath = readStringFromSettings("sourceFolder");
    ui->sourceFolderLineEdit->setText(QString::fromStdString(sourceFolderPath));
    destinationFolderPath = readStringFromSettings("destinationFolder");
    ui->destinationFolderLineEdit->setText(QString::fromStdString(destinationFolderPath));
    std::string enableLog = readStringFromSettings("enableLogging");
    if(r->size > 0){
        if(enableLog.compare("1") == 0){
            ui->enableLoggingCheckbox->setCheckState(Qt::Checked);
        }else if(enableLog.compare("0") == 0){
            ui->enableLoggingCheckbox->setCheckState(Qt::Unchecked);
        }
    }

    std::string replaceOriginals = readStringFromSettings("replaceOriginals");
    if(r->size > 0){
        if(replaceOriginals.compare("1") == 0){
            ui->replaceOriginalsCheckBox->setCheckState(Qt::Checked);
        }else if(replaceOriginals.compare("0") == 0){
            ui->replaceOriginalsCheckBox->setCheckState(Qt::Unchecked);
        }
    }

    ui->fbxFileTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->fbxFileTableWidget->setColumnWidth(0, ui->fbxFileTableWidget->width()*(0.666));
    ui->fbxFileTableWidget->setColumnWidth(1, ui->fbxFileTableWidget->width()*(0.333));
    ui->fbxFileTableWidget->verticalHeader()->setVisible(false);
    populateTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*This function is designed to be callable by other classes, in this case like Logger and FBXConverter to be able to write notifications to the user
about important info like if the Logger can't write to the directory it wants to or to tell the user something that may also be written to the log
like if an FBX failed to import or export*/
bool MainWindow::notifyUser(){
    return false;
}

void MainWindow::on_sourceFolderPushButton_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose a Folder as the Source Directory", QDir::homePath());
    ui->sourceFolderLineEdit->setText(folderName);
    sourceFolderPath = folderName.toStdString();
    ui->fbxFileTableWidget->setRowCount(0);
    QDir folder(QString::fromStdString(sourceFolderPath));
    QStringList filesList = folder.entryList(QStringList() << "*.fbx" << "*.FBX",QDir::Files);
    populateTable();
}


void MainWindow::on_logFolderPushButton_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose a Folder as the Log Directory", QDir::homePath());
    ui->logFolderLineEdit->setText(folderName);
    FBXFormatConverter::changeFBXLogDirectory(folderName.toStdString());
    logFilePath = folderName.toStdString();
}

void MainWindow::on_destinationFolderPushButton_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose a Folder as the Destination Directory", QDir::homePath());
    ui->destinationFolderLineEdit->setText(folderName);
    destinationFolderPath = folderName.toStdString();
}


//Copy check box saving functionality for the other checkbox

void MainWindow::on_enableLoggingCheckbox_stateChanged(int arg1)
{
    char c;
    if(arg1){
        FBXFormatConverter::enableFBXLogging(true);
        c = '1';
        fileSaver::save(settingsFile, &c, 1,  "enableLogging");
    }else{
        FBXFormatConverter::enableFBXLogging(false);
        c = '0';
        fileSaver::save(settingsFile, &c, 1,  "enableLogging");
    }
}

void MainWindow::on_replaceOriginalsCheckBox_stateChanged(int arg1)
{
    char c;
    if(arg1){
        c = '1';
        fileSaver::save(settingsFile, &c, 1,  "replaceOriginals");
    }else{
        c = '0';
        fileSaver::save(settingsFile, &c, 1,  "replaceOriginals");
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    //a crash is being caused somewhere in here, after files are being written, check for memory leaks
   /* saveStringtoSettings("logPath", logFilePath);
    saveStringtoSettings("sourceFolder", sourceFolderPath);
    saveStringtoSettings("destinationFolder", destinationFolderPath);*/
    event->accept();
}

void MainWindow::saveStringtoSettings(std::string recordID, std::string data){
    char saveBuff[200];
    memset(saveBuff, 0, 200);
    strcat(saveBuff, data.c_str());
    fileSaver::save(settingsFile, saveBuff, data.length(),  recordID);
}

std::string MainWindow::readStringFromSettings(std::string recordID){
    fileSaver::record* r;
    r = fileSaver::read(settingsFile, recordID);
    if(r->size > 0){
        return (char*)r->data;
    }else{
        return "";
    }
}

void MainWindow::populateTable(){
    if(sourceFolderPath.compare("") != 0){
        QDir folder(QString::fromStdString(sourceFolderPath));
        QStringList filesList = folder.entryList(QStringList() << "*.fbx" << "*.FBX",QDir::Files);
        int i = 0;
        foreach(QString file, filesList){
            ui->fbxFileTableWidget->insertRow(i);
            ui->fbxFileTableWidget->setItem(i, 0, new QTableWidgetItem(file));
            std::string fullPath;
            fullPath.append(sourceFolderPath);
            fullPath.append("/");
            fullPath.append(file.toStdString());
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
    ui->fbxFileTableWidget->clearSelection();
    if(ui->selectionTypeComboBox->currentText().compare("None") == 0){
        return;
    }
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
    for(int i = 0 ; i < ui->fbxFileTableWidget->rowCount(); i++){
        if(ui->fbxFileTableWidget->item(i, 0)->isSelected() && !ui->fbxFileTableWidget->item(i, 1)->isSelected()){
            ui->fbxFileTableWidget->item(i, 1)->setSelected(true);
        }else if(ui->fbxFileTableWidget->item(i, 1)->isSelected() && !ui->fbxFileTableWidget->item(i, 0)->isSelected()){
            ui->fbxFileTableWidget->item(i, 0)->setSelected(true);
        }
    }
}

