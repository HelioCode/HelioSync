#include "functions.h"

#include <QFileInfo>
#include <QHostInfo>
#include <QList>

#include "parser.h"
#include "serializer.h"

#ifdef __WIN32__
#define USER "USERNAME"
#define HOME "HOMEPATH"
#else
#define USER "USER"
#define HOME "HOME"
#endif

QString getRandomHex(int count)
{
    char itoh[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    QString result;
    float seed;
    int oldrand;
    int thetime;
    for(int i = 0; i < count; i++)
    {
        oldrand = rand();
        thetime = time(NULL);
        seed = (thetime / (i + 1 + oldrand)) * (thetime % ((i + 2) / (i / 3 + 1))) / (i / thetime + 1) * (oldrand % (i + 3));
        srand(round(seed));
        result += itoh[rand() % 16];
    }
    return result;
}

QString getUserName()
{
    return getenv(USER);
}

QString getHomePath()
{
    return QString(getenv(HOME)) + '/';
}

QString extractFileName(QString file)
{
    QFileInfo fi(file);
    return fi.fileName();
}

QString extractFilePath(QString file)
{
    QFileInfo fi(file);
    return fi.path();
}

QString getNetworkName()
{
    return QHostInfo::localHostName()
#ifdef __APPLE__
            .mid(0, QHostInfo::localHostName().size() - 6)
#endif
            ;
}

QString getLocalIp()
{
    QHostInfo local = QHostInfo::fromName(QHostInfo::localHostName());
    if (!local.addresses().isEmpty() && local.addresses().count() >= 2) {
        return local.addresses().value(1).toString();
    }
    return "";
}

QString createFunction(QString functionName, QMap<QString, QString> params)
{
    QVariantMap paramsVariant;
    QList<QString> paramsList = params.keys();
    for(unsigned int i = 0; i < paramsList.count(); i++)
    {
        paramsVariant[paramsList[i]] = params[paramsList[i]];
    }
    QVariantMap function;
    function["func"] = functionName;
    function["params"] = QVariantMap(paramsVariant);
    QJson::Serializer serializer;
    return serializer.serialize(function);
}

bool parseFunction(QString json, QString& funcName, QMap<QString, QString>& params)
{
    QJson::Parser parser;
    QVariant jsonVariant = parser.parse(json.toAscii());
    if(!jsonVariant.canConvert<QVariantMap>())
        return false;
    QVariantMap jsonVariantMap = jsonVariant.value<QVariantMap>();
    if(jsonVariantMap.find("func") == jsonVariantMap.end() || jsonVariantMap.find("params") == jsonVariantMap.end())
        return false;
    if(!jsonVariantMap.value("func").canConvert<QString>() || !jsonVariantMap.value("params").canConvert<QVariantMap>())
        return false;
    funcName = jsonVariantMap.value("func").value<QString>();
    QVariantMap resultVariantMap = jsonVariantMap.value("params").value<QVariantMap>();
    QList<QString> resultList = resultVariantMap.keys();
    for(unsigned int i = 0; i < resultList.count(); i++)
    {
        if(!resultVariantMap.value(resultList[i]).canConvert<QString>())
            continue;
        params[resultList[i]] = resultVariantMap.value(resultList[i]).value<QString>();
    }
    return true;
}
