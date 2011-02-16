#include <QtGui/QApplication>
#include "basecontroller.h"

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    baseController controller;

    return a.exec();
}
