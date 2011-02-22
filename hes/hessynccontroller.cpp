#include "hessynccontroller.h"

#include <QProcess>
#include <QTcpSocket>
#include <QVariantMap>
#include <QVariantList>
#include <QCryptographicHash>
#include <QHostAddress>
#include <QUdpSocket>

#include <math.h>

#include "typedef.h"

#include "functions.h"

#include "hesfunctionconnection.h"

#include <QtDebug>

HESSyncController::HESSyncController(QObject *parent) :
    QObject(parent)
{
    stopRequest = false;
    peerServer = new QTcpServer();
    connect(peerServer, SIGNAL(newConnection()), this, SLOT(handlePeerConnection()));
    peerServer->listen(QHostAddress::Any, PEERNOTIFYING_PORT);
}

bool HESSyncController::validateIp(QHostAddress ip)
{
    HESFunctionConnection functionConnection;
    if(!functionConnection.connectToPeer(ip))
        return false;
    return functionConnection.validate();
}

PeerInformation HESSyncController::retrieveInformation(QHostAddress ip)
{
    PeerInformation peerInformation;
    HESFunctionConnection functionConnection;
    if(!functionConnection.connectToPeer(ip))
        return peerInformation;
    functionConnection.getInformation(peerInformation.computerName, peerInformation.userName);
    return peerInformation;
}

void HESSyncController::getSyncablePeers()
{
    QUdpSocket* udpSocket = new QUdpSocket(this);
    udpSocket->writeDatagram(QByteArray("peer"), QHostAddress::Broadcast, 5678);

}

void HESSyncController::addIpToQueue(QHostAddress ip)
{
    if(validateIp(ip))
    {
        PeerInformation peerInformation = retrieveInformation(ip);
        emit foundSyncablePeer(ip.toString(), peerInformation.computerName, peerInformation.userName);
    }
}

void HESSyncController::handlePeerConnection()
{
    QTcpSocket* socket = peerServer->nextPendingConnection();
    if(validateIp(socket->peerAddress()))
    {
        PeerInformation peerInformation = retrieveInformation(socket->peerAddress());
        emit foundSyncablePeer(socket->peerAddress().toString(), peerInformation.computerName, peerInformation.userName);
    }
}
