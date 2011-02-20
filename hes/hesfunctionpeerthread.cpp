#include "hesfunctionpeerthread.h"

HESFunctionPeerThread::HESFunctionPeerThread(QObject *parent) :
    QThread(parent)
{
}

void HESFunctionPeerThread::run()
{
    functionPeer = new HESFunctionPeer();
    exec();
}
