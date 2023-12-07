#include "mainwindow.h"
#include <fbxformatconverter.h>
#include <QApplication>
#include "fileSaver.h"
#include <QDir>
#include <stdio.h>
int main(int argc, char *argv[])
{
   /* std::string helloWorld = "Hello World  OOOOOOOO SHIIIIII";
    std::string greeting = "HI!";
    std::string savePhrase = "Saving is Great!!";
    std::string readphrase = "I can change text too!!";
    char buff[80];
    memset(buff, 0, 80);
    strcat(buff, helloWorld.c_str());
    printf("Current Dir: %s\n", QDir::currentPath().toStdString().c_str());
    fileSaver::save(QDir::currentPath().toStdString().append("\\save.txt"), buff, helloWorld.length(),  "helloworld");
    printf("\n");
    memset(buff, 0, 80);
    strcat(buff, greeting.c_str());
    fileSaver::save(QDir::currentPath().toStdString().append("\\save.txt"), buff, greeting.length(),  "greet");

    printf("\n");
    memset(buff, 0, 80);
    strcat(buff, savePhrase.c_str());
    fileSaver::save(QDir::currentPath().toStdString().append("\\save.txt"), buff, savePhrase.length(),  "save");

    printf("\n");
    memset(buff, 0, 80);
    strcat(buff, readphrase.c_str());
    fileSaver::save(QDir::currentPath().toStdString().append("\\save.txt"), buff, readphrase.length(),  "read");

    fileSaver::record* r;
    r = fileSaver::read(QDir::currentPath().toStdString().append("\\save.txt"), "helloworld");
    memset(buff, '\0', 80);
    strcat(buff, (char*)r->data);
    printf("%d: %s\n", r->size, buff);*/
    FBXFormatConverter();
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("FBX Converter");
    w.show();
    return a.exec();
}
