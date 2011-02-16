#ifndef PEERWINDOW_H
#define PEERWINDOW_H

#include <QMainWindow>

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
    void displayProcessState(int process);
    void restore();
signals:
    void closed();
    void addIp(QString ip);
};

#endif // PEERWINDOW_H
