#ifndef HESSYNCCONTROLLER_H
#define HESSYNCCONTROLLER_H

#include <QObject>
#include <QList>
#include <QTcpServer>
#include <QHostAddress>
#include <QTimer>

#include "typedef.h"

class HESSyncController : public QObject
{
    Q_OBJECT
public:
    explicit HESSyncController(QObject *parent = 0);
    bool validateIp(QHostAddress ip);
    PeerInformation retrieveInformation(QHostAddress ip);
private:
    QTcpServer* peerServer;
    QTimer* timer;
    byte timerState;
    QList<PeerInformation> foundPeers;
    QList<PeerInformation> oldFoundPeers;
signals:
    void foundSyncablePeer(QString ip, QString computerName, QString userName);
    void removeSyncablePeer(QString ip);
public slots:
    void getSyncablePeers();
    void updateSyncablePeers();
    void addIpToQueue(QHostAddress ip);
    void handlePeerConnection();
    void stopGettingSyncableIps();
private slots:
    void timerInterval();
};

#endif // HESSYNCCONTROLLER_H
