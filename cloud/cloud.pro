#-------------------------------------------------
#
# Project created by QtCreator 2017-09-07T14:31:11
#
#-------------------------------------------------

QT       += core gui
QT      += sql

LIBS += -Iusr/include/mysql -lmysqlclient

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cloud
TEMPLATE = app

LIBS += -lpthread

CONFIG += console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    MHBoxLayout.cpp \
    Login.cpp \
    window.cpp \
    flowlayout.cpp \
    LoginWindow.cpp \
    UploadItem.cpp \
    MItem.cpp \
    MFileInfo.cpp \
    DataUtil.cpp \
    MFileItem.cpp \
    MDirectoryItem.cpp

HEADERS += \
    MHBoxLayout.h \
    Login.h \
    window.h \
    flowlayout.h \
    LoginWindow.h \
    UploadItem.h \
    MItem.h \
    MFileInfo.h \
    DataUtil.h \
    includes.h \
    MFileItem.h \
    MDirectoryItem.h

FORMS += \
        mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
