#include "functions.h"

#include <QFileInfo>
#include <QHostInfo>
#include <QList>

#include <time.h>

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

QByteArray StringMapToByteArray(QMap<QString, QString> map)
{
    QByteArray byteArray;
    QList<QString> keys;
    keys = map.keys();
    for(int i = 0; i < keys.count(); i++)
    {
        byteArray.append((unsigned char)keys[i].mid(0, 255).length());
        byteArray.append(keys[i].mid(0, 255));
        byteArray.append((unsigned char)map[keys[i]].mid(0, 255).length());
        byteArray.append(map[keys[i]].mid(0, 255));
    }
    return byteArray;
}

QMap<QString, QString> ByteArrayToStringMap(QByteArray byteArray)
{
    QMap<QString, QString> map;
    int pos = 0;
    int keyLength = 0;
    int stringLength = 0;
    int i;
    QString key;
    QString string;
    while(pos < byteArray.size())
    {
        key = "";
        string = "";
        keyLength = (unsigned char)byteArray[pos];
        for(i = pos + 1; i < pos + keyLength + 1; i++)
        {
            key += byteArray[i];
        }
        pos += keyLength + 1;
        stringLength = (unsigned char)byteArray[pos];
        for(i = pos + 1; i < pos + stringLength + 1; i++)
        {
            string += byteArray[i];
        }
        pos += stringLength + 1;
        map[key] = string;
    }
    return map;
}
