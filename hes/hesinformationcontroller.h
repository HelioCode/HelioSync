#ifndef HESInformationController_H
#define HESInformationController_H

#include <QObject>
#include <QList>
#include <QTcpServer>
#include <QHostAddress>
#include <QTimer>

#include "typedef.h"

class HESInformationController : public QObject
{
    Q_OBJECT
public:
    explicit HESInformationController(QObject *parent = 0);
    bool validateIp(QHostAddress address);
    PeerInformation retrieveInformation(QHostAddress address);
private:
    QTcpServer* peerServer;
    QTimer* timer;
    byte timerState;
    QList<PeerInformation> foundPeers;
    QList<PeerInformation> displayedPeers;
    QList<PeerInformation> addedPeers;
signals:
    void foundSyncablePeer(QString ip, QString computerName, QString userName);
    void removeSyncablePeer(QString ip);
    void syncablePeer(PeerInformation peerInformation);
    void rSyncablePeer(QHostAddress address);
public slots:
    void getSyncablePeers();
    void updateSyncablePeers();
    void addIp(QHostAddress ip);
    void handlePeerConnection();
    void stopGettingSyncableIps();
private slots:
    void timerInterval();
    void _syncablePeer(PeerInformation peerInformation);
    void _rSyncablePeer(QHostAddress address);
};

#endif // HESInformationController_H
