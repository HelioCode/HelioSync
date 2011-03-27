#ifndef HESInformationControllerTHREAD_H
#define HESInformationControllerTHREAD_H

#include <QThread>

#include "hesinformationcontroller.h"

class HESInformationControllerThread : public QThread
{
    Q_OBJECT
public:
    HESInformationController* informationController;
    explicit HESInformationControllerThread(QObject *parent = 0);
    void run();
signals:
    void setupDone();
public slots:
};

#endif // HESInformationControllerTHREAD_H
