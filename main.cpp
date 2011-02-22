#include <QtGui/QApplication>
#include "basecontroller.h"

#include <QHostAddress>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    qRegisterMetaType<QHostAddress>("QHostAddress");

    baseController controller;

    return a.exec();
}
