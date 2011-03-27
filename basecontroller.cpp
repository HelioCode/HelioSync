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

    informationControllerThread = new HESInformationControllerThread();
    informationControllerThread->start();
    connect(informationControllerThread, SIGNAL(setupDone()), this, SLOT(syncControllerSetupDone()));
}

void baseController::syncControllerSetupDone()
{
    informationController = informationControllerThread->informationController;
    connect(informationController, SIGNAL(foundSyncablePeer(QString, QString, QString)), peerWindow, SLOT(displaySyncablePeer(QString, QString, QString)), Qt::QueuedConnection);
    connect(informationController, SIGNAL(removeSyncablePeer(QString)), peerWindow, SLOT(removeSyncablePeer(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(getSyncablePeers()), informationController, SLOT(getSyncablePeers()), Qt::QueuedConnection);
    connect(peerWindow, SIGNAL(addIp(QHostAddress)), informationController, SLOT(addIp(QHostAddress)), Qt::QueuedConnection);
    connect(peerWindow, SIGNAL(updatePeers()), informationController, SLOT(updateSyncablePeers()), Qt::QueuedConnection);
}

void baseController::showPeerWindow()
{
    peerWindow->show();
    emit getSyncablePeers();
}

void baseController::peerWindowClosed()
{
    peerWindow->restore();
    informationController->stopGettingSyncableIps();
}

void baseController::syncPeer(QHostAddress peer)
{
    peerWindow->hide();
    syncWindow->show();
}
