#include "hesfunctionconnection.h"

#include "typedef.h"
#include "functions.h"

#include <QCryptographicHash>

HESFunctionConnection::HESFunctionConnection(QObject *parent) :
    QObject(parent)
{
    socket = NULL;
}

bool HESFunctionConnection::connectToPeer(QString ip)
{
    if(socket != NULL)
    {
        delete socket;
    }
    socket = new QTcpSocket();
    socket->connectToHost(ip, FUNCTIONPEER_PORT);
    if(!socket->waitForConnected(1000))
    {
        delete socket;
        return false;
    }
    return true;
}

QMap<QString, QString> HESFunctionConnection::request(QString functionName, QMap<QString, QString> params)
{
    QMap<QString, QString> resultMap;
    if(!socket->isOpen())
    {
        resultMap["error"] = "true";
        return resultMap;
    }
    QByteArray request;
    request.append((char)0);
    request.append((unsigned char)functionName.mid(0, 255).length());
    request.append(functionName);
    request.append(StringMapToByteArray(params));
    socket->write(request);
    socket->waitForBytesWritten(1000);
    socket->waitForReadyRead(1000);
    QByteArray response = socket->read(socket->bytesAvailable());
    if(response[0] == (char)0)
    {
        resultMap["error"] = "true";
        return resultMap;
    } else {
        int responseFunctionNameLength = request[1];
        QString responseFunctionName = "";
        for(int i = 2; i < responseFunctionNameLength + 2; i++)
        {
            responseFunctionName += request[i];
        }
        if(responseFunctionName == "error")
        {
            resultMap["error"] = "true";
            return resultMap;
        } else {
            resultMap = ByteArrayToStringMap(response.mid(responseFunctionNameLength + 2, response.size()));
        }
    }
    return resultMap;
}

//Function implementations

bool HESFunctionConnection::validate()
{
    QMap<QString, QString> params;
    QString randomHex = getRandomHex(8);
    params["key"] = randomHex;
    QString functionName = "validate";
    QMap<QString, QString> result = request(functionName, params);
    if(result["error"] == "true")
    {
        return false;
    } else {
        if(result["key"] != result["key"].mid(0, 8) + QCryptographicHash::hash(result["key"].mid(0, 8).toAscii() + ":HeliumSync:" + randomHex.toAscii(), QCryptographicHash::Md5).toHex())
            return false;
    }
    return true;
}

bool HESFunctionConnection::getInformation(QString& computerName, QString& userName)
{
    QMap<QString, QString> params;
    QString functionName = "getInformation";
    QMap<QString, QString> result = request(functionName, params);
    if(result["error"] == "true")
    {
        return false;
    } else {
        computerName = result["computerName"];
        userName = result["userName"];
    }
    return true;
}
