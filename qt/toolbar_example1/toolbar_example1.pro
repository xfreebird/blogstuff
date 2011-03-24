#-------------------------------------------------
#
# Project created by QtCreator 2011-03-24T10:10:55
#
#-------------------------------------------------

QT       += core gui

TARGET = toolbar_example1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    win7_include.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc

LIBS += libcomctl32 libole32
