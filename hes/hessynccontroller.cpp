#include "hessynccontroller.h"

#include <QProcess>
#include <QTcpSocket>
#include <QVariantMap>
#include <QVariantList>
#include <QCryptographicHash>
#include <QHostAddress>
#include <QUdpSocket>
#include <QTcpSocket>

#include <math.h>

#include "functions.h"

#include "hesfunctionconnection.h"

#include <QtDebug>

HESSyncController::HESSyncController(QObject *parent) :
    QObject(parent)
{
    peerServer = new QTcpServer();
    connect(peerServer, SIGNAL(newConnection()), this, SLOT(handlePeerConnection()));
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerInterval()));
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
    peerInformation.Ip = ip.toString();
    return peerInformation;
}

void HESSyncController::getSyncablePeers()
{
    QUdpSocket* udpSocket = new QUdpSocket(this);
    udpSocket->writeDatagram(QByteArray("peer"), QHostAddress::Broadcast, 5678);
    peerServer->listen(QHostAddress::Any, PEERNOTIFYING_PORT);
    timer->start(5000);
    timerState = 0;
}

void HESSyncController::addIpToQueue(QHostAddress ip)
{
    if(validateIp(ip))
    {
        PeerInformation peerInformation = retrieveInformation(ip);
        bool alreadyFoundPeer = false;
        foundPeers.append(peerInformation);
        for(unsigned int i = 0; i < oldFoundPeers.count(); i++)
        {
            if(oldFoundPeers[i].Ip == peerInformation.Ip)
            {
                alreadyFoundPeer = true;
                break;
            }
        }
        if(!alreadyFoundPeer)
        {
            emit foundSyncablePeer(peerInformation.Ip, peerInformation.computerName, peerInformation.userName);
        }
    }
}

void HESSyncController::handlePeerConnection()
{
    QTcpSocket* socket = peerServer->nextPendingConnection();
    if(validateIp(socket->peerAddress()))
    {
        PeerInformation peerInformation = retrieveInformation(socket->peerAddress());
        foundPeers.append(peerInformation);
        bool alreadyFoundPeer = false;
        for(unsigned int i = 0; i < oldFoundPeers.count(); i++)
        {
            if(oldFoundPeers[i].Ip == peerInformation.Ip)
            {
                alreadyFoundPeer = true;
                break;
            }
        }
        if(!alreadyFoundPeer)
        {
            emit foundSyncablePeer(peerInformation.Ip, peerInformation.computerName, peerInformation.userName);
        }
    }
}

void HESSyncController::timerInterval()
{
    if(timerState == 0)
    {
        peerServer->close();
        bool found = false;
        for(int i = oldFoundPeers.count() - 1; i >= 0; i--)
        {
            found = false;
            for(int j = foundPeers.count() - 1; j >= 0; j--)
            {
                if(oldFoundPeers[i].Ip == foundPeers[j].Ip)
                {
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                emit removeSyncablePeer(oldFoundPeers[i].Ip);
            }
        }
        oldFoundPeers = foundPeers;
        foundPeers.clear();
        timerState = 1;
    } else if(timerState == 1) {
        QUdpSocket* udpSocket = new QUdpSocket(this);
        udpSocket->writeDatagram(QByteArray("peer"), QHostAddress::Broadcast, 5678);
        peerServer->listen(QHostAddress::Any, PEERNOTIFYING_PORT);
        timerState = 0;
    }
}

void HESSyncController::updateSyncablePeers()
{
    timer->stop();
    peerServer->close();
    oldFoundPeers = foundPeers;
    QUdpSocket* udpSocket = new QUdpSocket(this);
    udpSocket->writeDatagram(QByteArray("peer"), QHostAddress::Broadcast, 5678);
    peerServer->listen(QHostAddress::Any, PEERNOTIFYING_PORT);
    timer->start(5000);
    timerState = 0;
}

void HESSyncController::stopGettingSyncableIps()
{
    timer->stop();
    peerServer->close();
    oldFoundPeers.clear();
    foundPeers.clear();
}
