#ifndef HESFUNCTIONPEER_H
#define HESFUNCTIONPEER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

class HESFunctionPeer : public QObject
{
    Q_OBJECT
public:
    explicit HESFunctionPeer(QObject *parent = 0);
private:
    QTcpServer* server;
    QUdpSocket* udpSocket;

    QMap<QString, QString> validate(QMap<QString, QString> params);
    QMap<QString, QString> getInformation(QMap<QString, QString> params);
signals:

public slots:
    void handleConnection();
    void processPendingDatagrams();
};

#endif // HESFUNCTIONPEER_H
