#include "hesfunctionpeer.h"

#include "../typedef.h"

#include <QCryptographicHash>
#include <QtDebug>

#include "functions.h"

HESFunctionPeer::HESFunctionPeer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(handleConnection()));
    server->listen(QHostAddress::Any, FUNCTIONPEER_PORT);
    udpSocket = new QUdpSocket();
    udpSocket->bind(BROADCAST_PORT, QUdpSocket::ShareAddress);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void HESFunctionPeer::handleConnection()
{
    QTcpSocket* socket = server->nextPendingConnection();
    socket->waitForReadyRead();
    QByteArray response;
    QByteArray request = socket->read(socket->bytesAvailable());
    if(request[0] == (char)1)
    {
        response.append((unsigned char)5);
        response.append("error");
    } else {
        int functionNameLength = request[1];
        QString functionName = "";
        for(int i = 2; i < functionNameLength + 2; i++)
        {
            functionName += request[i];
        }
        QMap<QString, QString> requestParams;
        if(request.length() > functionName.length() + 1)
        {
            requestParams = ByteArrayToStringMap(request.mid(functionNameLength + 2, request.size()));
        }

        response.append((char)1);
        response.append((unsigned char)functionName.mid(0, 255).length());
        response.append(functionName.mid(0,255));
        if(functionName == "validate")
        {
            response.append(StringMapToByteArray(validate(requestParams)));
        } else if(functionName == "getInformation")
        {
            response.append(StringMapToByteArray(getInformation(requestParams)));
        }
    }
    socket->write(response);
    socket->waitForBytesWritten();
}

void HESFunctionPeer::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress host;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &host);
        QTcpSocket* socket = new QTcpSocket();
        socket->connectToHost(host, PEERNOTIFYING_PORT);
        socket->disconnect();
    }
}

//Function implementations

QMap<QString, QString> HESFunctionPeer::validate(QMap<QString, QString> params)
{
    QMap<QString, QString> responseParams;
    QByteArray randomHex = getRandomHex(8).toAscii();
    responseParams["key"] = randomHex + QCryptographicHash::hash(randomHex + ":HeliumSync:" + params["key"].toAscii(), QCryptographicHash::Md5).toHex();
    return responseParams;
}

QMap<QString, QString> HESFunctionPeer::getInformation(QMap<QString, QString> params)
{
    QMap<QString, QString> responseParams;
    responseParams["userName"] = getUserName();
    responseParams["computerName"] = getNetworkName();
    return responseParams;
}
