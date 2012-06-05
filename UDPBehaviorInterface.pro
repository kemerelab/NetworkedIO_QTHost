#-------------------------------------------------
#
# Project created by QtCreator 2012-05-08T19:01:42
#
#-------------------------------------------------

QT       += core gui network

TARGET = UDPBehaviorInterface
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    behaviorcontrol.cpp \
    networkdatainterface.cpp \
    umazewidget.cpp \
    settingsdialog.cpp \
    wmazewidget.cpp \
    mazewidget.cpp

HEADERS  += mainwindow.h \
    behaviorcontrol.h \
    networkdatainterface.h \
    umazewidget.h \
    mazewidget.h \
    settingsdialog.h \
    wmazewidget.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

OTHER_FILES +=

RESOURCES += \
    InterfaceIcons.qrc
