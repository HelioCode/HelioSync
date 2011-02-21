#ifndef HESSYNCCONTROLLER_H
#define HESSYNCCONTROLLER_H

#include <QObject>
#include <QList>
#include <QTcpServer>
#include <QHostAddress>

struct PeerInformation
{
    QString Ip;
    QString computerName;
    QString userName;
};

class HESSyncController : public QObject
{
    Q_OBJECT
public:
    explicit HESSyncController(QObject *parent = 0);
    bool stopRequest;
    bool validateIp(QHostAddress ip);
    PeerInformation retrieveInformation(QHostAddress ip);
private:
    QList<QString> ipQueue;
    QTcpServer* peerServer;
signals:
    void foundSyncablePeer(QString ip, QString computerName, QString userName);
    void newProcessState(int process);
public slots:
    void getSyncablePeers();
    void addIpToQueue(QString ip);
    void handlePeerConnection();
};

#endif // HESSYNCCONTROLLER_H
