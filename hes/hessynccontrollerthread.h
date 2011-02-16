#ifndef HESSYNCCONTROLLERTHREAD_H
#define HESSYNCCONTROLLERTHREAD_H

#include <QThread>

#include "hessynccontroller.h"

class HESSyncControllerThread : public QThread
{
    Q_OBJECT
public:
    HESSyncController* syncController;
    explicit HESSyncControllerThread(QObject *parent = 0);
    void run();
signals:
    void setupDone();
public slots:
};

#endif // HESSYNCCONTROLLERTHREAD_H
