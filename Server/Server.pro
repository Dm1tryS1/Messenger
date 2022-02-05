QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
    myserver.cpp \
    myclient.cpp \
    dialog.cpp

HEADERS  += \
    myserver.h \
    dialog.h \
    myclient.h

FORMS    += dialog.ui














