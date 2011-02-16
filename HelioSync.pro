#-------------------------------------------------
#
# Project created by QtCreator 2011-02-01T17:13:33
#
#-------------------------------------------------

QT       += core gui network

TARGET = HelioSync
TEMPLATE = app

win32 {
  LIBS += -lqjson0
} else {
  LIBS += -lqjson
}

SOURCES +=  main.cpp\
            ./ui/peerwindow.cpp \
            basecontroller.cpp \
            ./hes/hesdaemonthread.cpp \
            ./hes/hessynccontrollerthread.cpp \
            ./hes/hessynccontroller.cpp \
            ./functions/functions.cpp \
            ./hes/hesdaemon.cpp \
            ./headers/*.cpp

HEADERS  += main.h\
            ./ui/peerwindow.h \
            basecontroller.h \
            ./hes/hesdaemonthread.h \
            ./hes/hessynccontrollerthread.h \
            ./hes/hessynccontroller.h \
            ./functions/functions.h \
            ./hes/hesdaemon.h \
            ./headers/*.h

FORMS    += ./ui/peerwindow.ui

RESOURCES += Ressources.qrc

INCLUDEPATH +=  ./headers/ \
                ./ui/ \
                ./hes/ \
                ./functions/
