#-------------------------------------------------
#
# Project created by QtCreator 2011-02-01T17:13:33
#
#-------------------------------------------------

QT       += core gui network

TARGET = HelioSync
TEMPLATE = app

SOURCES +=  main.cpp \
            ./ui/peerwindow.cpp \
            ./ui/syncwindow.cpp \
            ./basecontroller.cpp \
            ./hes/hesinformationcontrollerthread.cpp \
            ./hes/hesinformationcontroller.cpp \
            ./functions/functions.cpp \
            ./hes/hesfunctionpeer.cpp \
            ./hes/hesfunctionconnection.cpp \
            ./hes/hesfunctionpeerthread.cpp
HEADERS  += main.h\
            ./ui/peerwindow.h \
            ./ui/syncwindow.h \
            ./basecontroller.h \
            ./hes/hesinformationcontrollerthread.h \
            ./hes/hesinformationcontroller.h \
            ./functions/functions.h \
            ./typedef.h \
            ./hes/hesfunctionpeer.h \
            ./hes/hesfunctionconnection.h \
            ./hes/hesfunctionpeerthread.h
FORMS    += ./ui/peerwindow.ui \
            ./ui/syncwindow.ui

RESOURCES += Ressources.qrc

INCLUDEPATH +=  ./ui/ \
                ./hes/ \
                ./functions/ \
                ./headers/
