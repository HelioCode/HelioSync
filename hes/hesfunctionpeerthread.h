#ifndef HESFUNCTIONPEERTHREAD_H
#define HESFUNCTIONPEERTHREAD_H

#include <QThread>

#include "hesfunctionpeer.h"

class HESFunctionPeerThread : public QThread
{
    Q_OBJECT
public:
    explicit HESFunctionPeerThread(QObject *parent = 0);
    void run();
private:
    HESFunctionPeer* functionPeer;
signals:

public slots:

};

#endif // HESFUNCTIONPEERTHREAD_H
