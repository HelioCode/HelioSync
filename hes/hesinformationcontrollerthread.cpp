#include "hesinformationcontrollerthread.h"

#include <QProcess>

HESInformationControllerThread::HESInformationControllerThread(QObject *parent) :
    QThread(parent)
{
}

void HESInformationControllerThread::run()
{
    syncController = new HESInformationController();
    emit setupDone();
    exec();
}
