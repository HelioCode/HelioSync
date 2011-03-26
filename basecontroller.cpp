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
    connect(syncAction, SIGNAL(triggered()), this, SLOT(showPeerWindow()));
    trayMenu->addAction(syncAction);
#ifndef __APPLE__
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(showPeerWindow()));
#endif

    trayMenu->addSeparator();

    QAction* quitAction = new QAction("Quit", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    trayMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayMenu);

    trayIcon->show();

    functionPeer = new HESFunctionPeerThread();
    functionPeer->start();

    peerWindow = new PeerWindow();
    connect(peerWindow, SIGNAL(closed()), this, SLOT(peerWindowClosed()));
    connect(peerWindow, SIGNAL(syncPeer(QHostAddress)), this, SLOT(syncPeer(QHostAddress)));

    syncWindow = new SyncWindow();

    syncControllerThread = new HESInformationControllerThread();
    syncControllerThread->start();
    connect(syncControllerThread, SIGNAL(setupDone()), this, SLOT(syncControllerSetupDone()));
}

void baseController::syncControllerSetupDone()
{
    syncController = syncControllerThread->syncController;
    connect(syncController, SIGNAL(foundSyncablePeer(QString, QString, QString)), peerWindow, SLOT(displaySyncablePeer(QString, QString, QString)), Qt::QueuedConnection);
    connect(syncController, SIGNAL(removeSyncablePeer(QString)), peerWindow, SLOT(removeSyncablePeer(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(getSyncablePeers()), syncController, SLOT(getSyncablePeers()), Qt::QueuedConnection);
    connect(peerWindow, SIGNAL(addIp(QHostAddress)), syncController, SLOT(addIp(QHostAddress)), Qt::QueuedConnection);
    connect(peerWindow, SIGNAL(updatePeers()), syncController, SLOT(updateSyncablePeers()), Qt::QueuedConnection);
}

void baseController::showPeerWindow()
{
    peerWindow->show();
    emit getSyncablePeers();
}

void baseController::peerWindowClosed()
{
    peerWindow->restore();
    syncController->stopGettingSyncableIps();
}

void baseController::syncPeer(QHostAddress peer)
{
    peerWindow->hide();
    syncWindow->show();
}
