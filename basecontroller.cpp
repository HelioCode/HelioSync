#include "basecontroller.h"

#include <QApplication>
#include <QtDebug>

baseController::baseController(QObject *parent) :
    QObject(parent)
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/HeS_Icon.png"));

    trayMenu = new QMenu();

    QAction* syncAction = new QAction("Synchronize", this);
    connect(syncAction, SIGNAL(triggered()), this, SLOT(startSync()));
    trayMenu->addAction(syncAction);
#ifndef __APPLE__
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(startSync()));
#endif

    trayMenu->addSeparator();

    QAction* quitAction = new QAction("Quit", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    trayMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayMenu);

    trayIcon->show();

    peerWindowOpen = false;

    listener = new HESDaemonThread();
    listener->start();

    peerWindow = new PeerWindow();
    connect(peerWindow, SIGNAL(closed()), this, SLOT(peerWindowClosed()));
    syncControllerThread = new HESSyncControllerThread();
    syncControllerThread->start();
    connect(syncControllerThread, SIGNAL(setupDone()), this, SLOT(syncControllerSetupDone()));
}

void baseController::syncControllerSetupDone()
{
    syncController = syncControllerThread->syncController;
    connect(syncController, SIGNAL(newProcessState(int)), peerWindow, SLOT(displayProcessState(int)), Qt::QueuedConnection);
    connect(syncController, SIGNAL(foundSyncablePeer(QString, QString, QString)), peerWindow, SLOT(displaySyncablePeer(QString, QString, QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(getSyncablePeers()), syncController, SLOT(getSyncablePeers()), Qt::QueuedConnection);
    connect(peerWindow, SIGNAL(addIp(QString)), syncController, SLOT(addIpToQueue(QString)), Qt::DirectConnection);
}

void baseController::startSync()
{
    peerWindow->show();
    emit getSyncablePeers();
}

void baseController::peerWindowClosed()
{
    peerWindow->restore();
    syncController->stopRequest = true;
}
