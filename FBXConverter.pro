QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fbxformatconverter.cpp \
    filesaver.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    fbxformatconverter.h \
    filesaver.h \
    logger.h \
    mainwindow.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../../Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/release/' -llibfbxsdk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../../Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/debug/' -llibfbxsdk
else:unix: LIBS += -L$$PWD/'../../../../../Program Files/Autodesk/FBX/FBX SDK/2020.3.4/lib/vs2022/x64/' -llibfbxsdk

INCLUDEPATH += $$PWD/'../../../../../Program Files/Autodesk/FBX/FBX SDK/2020.3.4/include'
DEPENDPATH += $$PWD/'../../../../../Program Files/Autodesk/FBX/FBX SDK/2020.3.4/include'
