#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef unsigned char byte;

#define FUNCTIONPEER_PORT 6431
#define PEERNOTIFYING_PORT 6432
#define LISTEN_PORT 6421
#define BROADCAST_PORT 6421

#include <QtGui>
#include <QHostAddress>

struct PeerInformation
{
    QHostAddress address;
    QString computerName;
    QString userName;
};

#endif //TYPEDEF_H
