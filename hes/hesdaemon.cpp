#include "hesdaemon.h"

#include "typedef.h"

#include <QTcpSocket>
#include <QCryptographicHash>

#include "parser.h"
#include "serializer.h"

#include "functions.h"

HESDaemon::HESDaemon(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(handleConnection()));
    server->listen(QHostAddress::Any, LISTEN_PORT);
}

void HESDaemon::handleConnection()
{
    QTcpSocket* socket = server->nextPendingConnection();
    socket->waitForReadyRead();
    QByteArray data = socket->read(socket->bytesAvailable());
    QString functionName;
    QMap<QString, QString> functionParams;
    parseFunction(data, functionName, functionParams);
    if(functionName == "validate")
    {
        QByteArray randomHex = getRandomHex(8).toAscii();
        QMap<QString, QString> answerParams;
        answerParams["retkey"] = randomHex + QCryptographicHash::hash(randomHex + ":HeliumSync:" + functionParams["key"].toAscii(), QCryptographicHash::Md5).toHex();
        socket->write(createFunction("validate", answerParams).toAscii());
    } else if(functionName == "getInformation")
    {
        QMap<QString, QString> answerParams;
        answerParams["computerName"] = getNetworkName();
        answerParams["userName"] = getUserName();
        socket->write(createFunction("getInformation", answerParams).toAscii());
    }
    socket->waitForBytesWritten();
}
