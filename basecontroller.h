#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>

#include "peerwindow.h"
#include "hesdaemonthread.h"
#include "hessynccontrollerthread.h"

class baseController : public QObject
{
    Q_OBJECT
public:
    explicit baseController(QObject *parent = 0);
private:
    QSystemTrayIcon* trayIcon;
    QMenu* trayMenu;
    PeerWindow* peerWindow;
    bool peerWindowOpen;
    HESDaemonThread* listener;
    HESSyncController* syncController;
    HESSyncControllerThread* syncControllerThread;
signals:
    void getSyncablePeers();
public slots:
    void syncControllerSetupDone();
    void startSync();
    void peerWindowClosed();
};

#endif // BASECONTROLLER_H
