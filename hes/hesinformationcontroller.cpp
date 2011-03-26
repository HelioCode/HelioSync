#include "hesinformationcontroller.h"

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

HESInformationController::HESInformationController(QObject *parent) :
    QObject(parent)
{
    peerServer = new QTcpServer();
    connect(peerServer, SIGNAL(newConnection()), this, SLOT(handlePeerConnection()));
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerInterval()));
    connect(this, SIGNAL(syncablePeer(PeerInformation)), this, SLOT(_syncablePeer(PeerInformation)));
    connect(this, SIGNAL(rSyncablePeer(QHostAddress)), this, SLOT(_rSyncablePeer(QHostAddress)));
}

bool HESInformationController::validateIp(QHostAddress ip)
{
    HESFunctionConnection functionConnection;
    if(!functionConnection.connectToPeer(ip))
        return false;
    return functionConnection.validate();
}

PeerInformation HESInformationController::retrieveInformation(QHostAddress address)
{
    PeerInformation peerInformation;
    HESFunctionConnection functionConnection;
    if(!functionConnection.connectToPeer(address))
        return peerInformation;
    functionConnection.getInformation(peerInformation.computerName, peerInformation.userName);
    peerInformation.address = address;
    return peerInformation;
}

void HESInformationController::getSyncablePeers()
{
    QUdpSocket* udpSocket = new QUdpSocket(this);
    udpSocket->writeDatagram(QByteArray("peer"), QHostAddress::Broadcast, 5678);
    peerServer->listen(QHostAddress::Any, PEERNOTIFYING_PORT);
    timer->start(5000);
    timerState = 0;
}

void HESInformationController::addIp(QHostAddress address)
{
    if(validateIp(address))
    {
        PeerInformation peerInformation = retrieveInformation(address);
        foundPeers.append(peerInformation);
        addedPeers.append(peerInformation);
        emit syncablePeer(peerInformation);
    }
}

void HESInformationController::handlePeerConnection()
{
    QTcpSocket* socket = peerServer->nextPendingConnection();
    if(validateIp(socket->peerAddress()))
    {
        PeerInformation peerInformation = retrieveInformation(socket->peerAddress());
        foundPeers.append(peerInformation);
        emit syncablePeer(peerInformation);
    }
}

void HESInformationController::timerInterval()
{
    if(timerState == 0)
    {
        peerServer->close();
        bool foundPeer = false;
        QList<int> toRemove;
        for(int i = 0; i < displayedPeers.count(); i++)
        {
            foundPeer = false;
            for(int j = 0; j < foundPeers.count(); j++)
            {
                if(displayedPeers[i].address.toString() == foundPeers[j].address.toString())
                {
                    foundPeer = true;
                    break;
                }
            }
            if(!foundPeer)
            {
                toRemove.append(i);
            }
        }
        for(int i = 0; i < toRemove.count(); i++)
        {
            emit rSyncablePeer(displayedPeers[toRemove[i]].address);
        }
        foundPeers.clear();
        timerState = 1;
    } else if(timerState == 1) {
        QUdpSocket* udpSocket = new QUdpSocket(this);
        udpSocket->writeDatagram(QByteArray("peer"), QHostAddress::Broadcast, 5678);
        for(int i = 0; i < addedPeers.count(); i++)
        {
            udpSocket->writeDatagram(QByteArray("peer"), addedPeers[i].address, 5678);
        }
        peerServer->listen(QHostAddress::Any, PEERNOTIFYING_PORT);
        timerState = 0;
    }
}

void HESInformationController::_syncablePeer(PeerInformation peerInformation)
{
    bool peerDisplayed = false;
    for(int i = 0; i < displayedPeers.count(); i++)
    {
        if(displayedPeers[i].address.toString() == peerInformation.address.toString())
        {
            peerDisplayed = true;
            break;
        }
    }
    if(!peerDisplayed)
    {
        displayedPeers.append(peerInformation);
        emit foundSyncablePeer(peerInformation.address.toString(), peerInformation.computerName, peerInformation.userName);
    }
}

void HESInformationController::_rSyncablePeer(QHostAddress address)
{
    QList<int> toDelete;
    for(int i = 0; i < displayedPeers.count(); i++)
    {
        if(displayedPeers[i].address.toString() == address.toString())
        {
            toDelete.append(i);
            emit removeSyncablePeer(displayedPeers[i].address.toString());
        }
    }
    for(int i = 0; i < toDelete.count(); i++)
    {
        displayedPeers.removeAt(toDelete[i]);
    }
}

void HESInformationController::updateSyncablePeers()
{
    timer->stop();
    peerServer->close();
    QUdpSocket* udpSocket = new QUdpSocket(this);
    udpSocket->writeDatagram(QByteArray("peer"), QHostAddress::Broadcast, 5678);
    peerServer->listen(QHostAddress::Any, PEERNOTIFYING_PORT);
    timer->start(5000);
    timerState = 0;
}

void HESInformationController::stopGettingSyncableIps()
{
    timer->stop();
    peerServer->close();
    displayedPeers.clear();
    foundPeers.clear();
    foundPeers.clear();
}
