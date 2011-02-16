#ifndef HESSYNCCONTROLLER_H
#define HESSYNCCONTROLLER_H

#include <QObject>
#include <QList>

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
    bool validateIp(QString ip);
    PeerInformation retrieveInformation(QString ip);
private:
    QList<QString> ipQueue;
signals:
    void foundSyncablePeer(QString ip, QString computerName, QString userName);
    void newProcessState(int process);
public slots:
    void getSyncablePeers();
    void addIpToQueue(QString ip);
};

#endif // HESSYNCCONTROLLER_H
