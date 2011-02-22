#ifndef PEERWINDOW_H
#define PEERWINDOW_H

#include <QMainWindow>
#include <QHostAddress>

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
    void displaySyncablePeer(QString ip, QString computerName, QString userName);
    void restore();
signals:
    void closed();
    void addIp(QHostAddress ip);
};

#endif // PEERWINDOW_H
