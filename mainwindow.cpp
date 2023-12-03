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
char saveBuff[200];


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileSaver::record* r;
    r = fileSaver::read(settingsFile, "logPath");
    if(r->size > 0){
        memset(saveBuff, '\0', 200);
        strcat(saveBuff, (char*)r->data);
        logFilePath = saveBuff;
        ui->logFolderLineEdit->setText(QString::fromStdString(logFilePath));
    }
    r = fileSaver::read(settingsFile, "sourceFolder");
    if(r->size > 0){
        memset(saveBuff, '\0', 200);
        strcat(saveBuff, (char*)r->data);
        sourceFolderPath = saveBuff;
        ui->sourceFolderLineEdit->setText(QString::fromStdString(sourceFolderPath));
    }
    r = fileSaver::read(settingsFile, "destinationFolder");
    if(r->size > 0){
        memset(saveBuff, '\0', 200);
        strcat(saveBuff, (char*)r->data);
        destinationFolderPath = saveBuff;
        ui->destinationFolderLineEdit->setText(QString::fromStdString(destinationFolderPath));
    }
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


void MainWindow::on_enableLoggingCheckbox_stateChanged(int arg1)
{
    if(arg1){
        FBXFormatConverter::enableFBXLogging(true);
    }else{
        FBXFormatConverter::enableFBXLogging(false);
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    memset(saveBuff, 0, 200);
    strcat(saveBuff, logFilePath.c_str());
    fileSaver::save(settingsFile, saveBuff, logFilePath.length(),  "logPath");
    memset(saveBuff, 0, 200);
    strcat(saveBuff, sourceFolderPath.c_str());
    fileSaver::save(settingsFile, saveBuff, sourceFolderPath.length(),  "sourceFolder");
    memset(saveBuff, 0, 200);
    strcat(saveBuff, destinationFolderPath.c_str());
    fileSaver::save(settingsFile, saveBuff, destinationFolderPath.length(),  "destinationFolder");
    event->accept();
}
