#include "HESDaemonThread.h"

#include <QTcpSocket>
#include <QDebug>

HESDaemonThread::HESDaemonThread(QObject *parent) :
    QThread(parent)
{
}

void HESDaemonThread::run()
{
    daemon = new HESDaemon();
    exec();
}
