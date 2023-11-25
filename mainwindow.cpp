#include "mainwindow.h"
#include "ui_mainwindow.h"

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
