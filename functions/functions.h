#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtGui>
#include <QMap>

QString getRandomHex(int count);
QString getUserName();
QString getHomePath();
QString extractFileName(QString file);
QString extractFilePath(QString file);
QString getNetworkName();
QString getLocalIp();

QString createFunction(QString functionName, QMap<QString, QString> params);
bool parseFunction(QString json, QString& funcName, QMap<QString, QString>& params);


#endif //FUNCTIONS_H
