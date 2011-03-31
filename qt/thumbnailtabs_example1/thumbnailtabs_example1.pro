#-------------------------------------------------
#
# Project created by QtCreator 2011-03-31T11:22:21
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = thumbnailtabs_example1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    win7utils.cpp \
    tabsmanager.cpp \
    webbrowserwidget.cpp

HEADERS  += mainwindow.h \
    win7utils.h \
    tabsmanager.h \
    webbrowserwidget.h

FORMS    += mainwindow.ui \
    webbrowserwidget.ui

LIBS += libgdi32 libole32
#libqwin7utils libuser32 libole32 libshell32 libshlwapi libcomctl32 libgdi32

RESOURCES += \
    icons.qrc
