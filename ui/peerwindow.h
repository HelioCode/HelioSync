#ifndef PEERWINDOW_H
#define PEERWINDOW_H

#include <QMainWindow>
#include <QHostAddress>

#include "typedef.h"

namespace Ui {
    class PeerWindow;
}

class PeerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PeerWindow(QWidget *parent = 0);
    ~PeerWindow();
private:
    Ui::PeerWindow *ui;
    void closeEvent(QCloseEvent *);
public slots:
    void on_pbAddIp_clicked();
    void on_twIps_clicked();
    void on_pbUpdate_clicked();
    void displaySyncablePeer(QString ip, QString computerName, QString userName);
    void removeSyncablePeer(QString ip);
    void restore();
signals:
    void closed();
    void addIp(QHostAddress ip);
    void updatePeers();
};

#endif // PEERWINDOW_H
