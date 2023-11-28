#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <fbxformatconverter.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
}


void MainWindow::on_logFolderPushButton_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose a Folder as the Log Directory", QDir::homePath());
    ui->logFolderLineEdit->setText(folderName);
    FBXFormatConverter::changeFBXLogDirectory(folderName.toStdString());
}


void MainWindow::on_destinationFolderPushButton_clicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose a Folder as the Destination Directory", QDir::homePath());
    ui->destinationFolderLineEdit->setText(folderName);
}


void MainWindow::on_enableLoggingCheckbox_stateChanged(int arg1)
{
    if(arg1){
        FBXFormatConverter::enableFBXLogging(true);
    }else{
        FBXFormatConverter::enableFBXLogging(false);
    }
}

