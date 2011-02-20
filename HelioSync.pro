#-------------------------------------------------
#
# Project created by QtCreator 2011-02-01T17:13:33
#
#-------------------------------------------------

QT       += core gui network

TARGET = HelioSync
TEMPLATE = app

SOURCES +=  main.cpp\
            ./ui/peerwindow.cpp \
            ./basecontroller.cpp \
            ./hes/hessynccontrollerthread.cpp \
            ./hes/hessynccontroller.cpp \
            ./functions/functions.cpp \
            ./hes/hesfunctionpeer.cpp \
            ./hes/hesfunctionconnection.cpp \
            ./hes/hesfunctionpeerthread.cpp
HEADERS  += main.h\
            ./ui/peerwindow.h \
            ./basecontroller.h \
            ./hes/hessynccontrollerthread.h \
            ./hes/hessynccontroller.h \
            ./functions/functions.h \
            ./headers/*.h \
            ./typedef.h \
            ./hes/hesfunctionpeer.h \
            ./hes/hesfunctionconnection.h \
            ./hes/hesfunctionpeerthread.h

FORMS    += ./ui/peerwindow.ui

RESOURCES += Ressources.qrc\

INCLUDEPATH +=  ./ui/ \
                ./hes/ \
                ./functions/
