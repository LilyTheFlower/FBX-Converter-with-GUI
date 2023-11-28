#include "mainwindow.h"
#include <fbxformatconverter.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    FBXFormatConverter();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
