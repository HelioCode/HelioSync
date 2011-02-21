#include "hessynccontroller.h"

#include <QProcess>
#include <QTcpSocket>
#include <QVariantMap>
#include <QVariantList>
#include <QCryptographicHash>
#include <QHostAddress>

#include <math.h>

#include "typedef.h"

#include "functions.h"

#include "hesfunctionconnection.h"

#include <QtDebug>

HESSyncController::HESSyncController(QObject *parent) :
    QObject(parent)
{
    stopRequest = false;
}

bool HESSyncController::validateIp(QString ip)
{
    QProcess* process = new QProcess(this);
    QStringList args;
#ifndef __WIN32__
    args << "-c" << "1" << "-W" << "1" << ip;
#else
    args << "-n" << "1" << "-w" << "1" << ip;
#endif
    process->start("ping", args);
    process->waitForFinished(5000);
    QString output = process->readAllStandardOutput();
    if(output[output.length() - 3] != 'm')
        return false;

    HESFunctionConnection functionConnection;
    if(!functionConnection.connectToPeer(ip))
        return false;
    return functionConnection.validate();
}

PeerInformation HESSyncController::retrieveInformation(QString ip)
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
    bool validation;
    QString ip;
    int i = 1;
    QString subnet = getLocalIp();
    for(unsigned int i = subnet.length() - 1; i >= 0; i--)
    {
        if(subnet[i] == '.')
        {
            subnet = subnet.mid(0, i + 1);
            break;
        }
    }
    while(i <= 255)
    {
        if(stopRequest)
            break;
        if(ipQueue.count() != 0)
        {
            ip = ipQueue[0];
            ipQueue.removeFirst();
        } else {
            ip = subnet + QString::number(i);
            i++;
        }
        validation = validateIp(ip);
        if(stopRequest)
            break;
        emit newProcessState(round(i/255.0*1000.0));
        if(validation)
        {
            PeerInformation peerInformation = retrieveInformation(ip);
            emit foundSyncablePeer(ip, peerInformation.computerName, peerInformation.userName);
        } else {
            continue;
        }
    }
    stopRequest = false;
}

void HESSyncController::addIpToQueue(QString ip)
{
    ipQueue.append(ip);
}
