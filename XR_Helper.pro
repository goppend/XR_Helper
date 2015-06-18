#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T17:58:28
#
#-------------------------------------------------

QT       += core gui\
            widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XR_Helper
TEMPLATE = app

INCLUDEPATH += C:/Users/goppend/Documents/asdfwfsda/libsquish

SOURCES += main.cpp\
		MainWindow.cpp\
        VArrowLineEdit.cpp\
        VLogger.cpp\
        CEntryLTX.cpp\
        CFileDDS.cpp \
    CFileLTX.cpp

HEADERS  += MainWindow.h\
        VArrowLineEdit.h\
        VLogger.h\
        CEntryLTX.h\
        CFileDDS.h \
    CFileLTX.h

LIBS += /usr/lib/libsquish.a

RESOURCES += icons.qrc

FORMS     += XR_Helper.ui
