#include "mainwindow.h"
#include <fbxformatconverter.h>
#include <QApplication>
#include "fileSaver.h"
#include <QDir>
#include <stdio.h>
int main(int argc, char *argv[])
{
    FBXFormatConverter();
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("FBX Converter");
    w.setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    w.show();
    return a.exec();
}
