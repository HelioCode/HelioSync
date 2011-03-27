#include "hesinformationcontrollerthread.h"

#include <QProcess>

HESInformationControllerThread::HESInformationControllerThread(QObject *parent) :
    QThread(parent)
{
}

void HESInformationControllerThread::run()
{
    informationController = new HESInformationController();
    emit setupDone();
    exec();
}
