#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>

#include "peerwindow.h"
#include "syncwindow.h"
#include "hesfunctionpeerthread.h"
#include "hesinformationcontrollerthread.h"

class baseController : public QObject
{
    Q_OBJECT
public:
    explicit baseController(QObject *parent = 0);
private:
    QSystemTrayIcon* trayIcon;
    QMenu* trayMenu;
    PeerWindow* peerWindow;
    SyncWindow* syncWindow;
    HESFunctionPeerThread* functionPeer;
    HESInformationController* informationController;
    HESInformationControllerThread* informationControllerThread;
signals:
    void getSyncablePeers();
public slots:
    void syncControllerSetupDone();
    void showPeerWindow();
    void peerWindowClosed();
    void syncPeer(QHostAddress peer);
};

#endif // BASECONTROLLER_H
