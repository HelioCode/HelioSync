#include "hessynccontrollerthread.h"

#include <QProcess>

HESSyncControllerThread::HESSyncControllerThread(QObject *parent) :
    QThread(parent)
{
}

void HESSyncControllerThread::run()
{
    syncController = new HESSyncController();
    emit setupDone();
    exec();
}
