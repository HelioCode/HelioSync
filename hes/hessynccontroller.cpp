#include "hessynccontroller.h"

#include <QProcess>
#include <QTcpSocket>
#include <QVariantMap>
#include <QVariantList>
#include <QCryptographicHash>
#include <QtDebug>
#include <QHostAddress>

#include <math.h>

#include "typedef.h"

#include "serializer.h"
#include "parser.h"

#include "functions.h"

HESSyncController::HESSyncController(QObject *parent) :
    QObject(parent)
{
    stopRequest = false;
}

bool HESSyncController::validateIp(QString ip)
{
    QProcess* process = new QProcess(this);
    QStringList args;
    args << "-c" << "1" << "-W" << "1" << ip;
    process->start("ping", args);
    process->waitForFinished(5000);
    QString output = process->readAllStandardOutput();
    if(output[output.length() - 3] != 'm')
        return false;

    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(ip, LISTEN_PORT);
    if(!socket->waitForConnected(1000))
        return false;
    QMap<QString, QString> validationParams;
    QString randomHex = getRandomHex(8);
    validationParams["key"] = randomHex;
    socket->write(createFunction("validate", validationParams).toAscii());
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    QString responseFunction;
    QMap<QString, QString> responseMap;
    parseFunction(socket->read(socket->bytesAvailable()), responseFunction, responseMap);
    if(responseMap["retkey"] != responseMap["retkey"].mid(0, 8) + QCryptographicHash::hash((responseMap["retkey"].mid(0, 8) + ":HeliumSync:" + randomHex).toAscii(), QCryptographicHash::Md5).toHex())
        return false;
    return true;
}

PeerInformation HESSyncController::retrieveInformation(QString ip)
{
    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(ip, LISTEN_PORT);
    if(!socket->waitForConnected(1000))
        return PeerInformation();
    QMap<QString, QString> requestParams;
    socket->write(createFunction("getInformation", requestParams).toAscii());
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    QString responseFunction;
    QMap<QString, QString> responseMap;
    parseFunction(socket->read(socket->bytesAvailable()), responseFunction, responseMap);
    PeerInformation peerInformation;
    peerInformation.Ip = ip;
    peerInformation.computerName = responseMap["computerName"];
    peerInformation.userName = responseMap["userName"];
    return peerInformation;
}

void HESSyncController::getSyncablePeers()
{
    bool validation;
    QString ip;
    int i = 1;
    while(i <= 255)
    {
        if(stopRequest)
            break;
        if(ipQueue.count() != 0)
        {
            ip = ipQueue[0];
            ipQueue.removeFirst();
        } else {
            ip = "192.168.0." + QString::number(i);
            i++;
        }
        validation = validateIp(ip);
        if(stopRequest)
            break;
        emit newProcessState(round(i/255.0*1000.0));
        if(validation)
        {
            PeerInformation peerInformation = retrieveInformation(ip);
            emit foundSyncablePeer(peerInformation.Ip, peerInformation.computerName, peerInformation.userName);
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
