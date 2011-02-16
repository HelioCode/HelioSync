#ifndef HESDAEMONTHREAD_H
#define HESDAEMONTHREAD_H

#include <QThread>

#include "hesdaemon.h"

class HESDaemonThread : public QThread
{
    Q_OBJECT
public:
    explicit HESDaemonThread(QObject *parent = 0);
    void run();
private:
    HESDaemon* daemon;
signals:

public slots:
};

#endif // HESDAEMONTHREAD_H
