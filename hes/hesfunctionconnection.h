#ifndef HESFUNCTIONCONNECTION_H
#define HESFUNCTIONCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QMap>
#include <QHostAddress>

class HESFunctionConnection : public QObject
{
    Q_OBJECT
public:
    explicit HESFunctionConnection(QObject *parent = 0);
    bool connectToPeer(QHostAddress ip);

    bool validate();
    bool getInformation(QString& computerName, QString& userName);
private:
    QTcpSocket* socket;
    QMap<QString, QString> request(QString functionName, QMap<QString, QString> params);
signals:

public slots:

};

#endif // HESFUNCTIONCONNECTION_H
