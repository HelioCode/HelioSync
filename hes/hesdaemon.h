#ifndef HESDAEMON_H
#define HESDAEMON_H

#include <QObject>
#include <QTcpServer>

class HESDaemon : public QObject
{
    Q_OBJECT
public:
    explicit HESDaemon(QObject *parent = 0);
private:
    QTcpServer* server;
signals:

public slots:
    void handleConnection();
};

#endif // HESDAEMON_H
