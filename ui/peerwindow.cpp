#include "peerwindow.h"
#include "ui_peerwindow.h"

#include <QInputDialog>
#include <QtDebug>

PeerWindow::PeerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PeerWindow)
{
    ui->setupUi(this);
    QStringList newItem;
    newItem << "192.168.0.255" << "" << "";
    ui->twIps->addTopLevelItem(new QTreeWidgetItem(newItem, 0));
    ui->twIps->resizeColumnToContents(0);
    ui->twIps->clear();
}

void PeerWindow::closeEvent(QCloseEvent *)
{
    emit closed();
}

PeerWindow::~PeerWindow()
{
    delete ui;
}

void PeerWindow::on_pbAddIp_clicked()
{
    QString ip = QInputDialog::getText(this, "Enter IP", "Enter IP:");
    emit addIp(QHostAddress(ip));
}

void PeerWindow::on_twIps_clicked()
{
    ui->pbContinue->setEnabled(true);
}

void PeerWindow::on_pbUpdate_clicked()
{
    emit updatePeers();
}

void PeerWindow::displaySyncablePeer(QString ip, QString computerName, QString userName)
{
    QStringList newItem;
    newItem << ip << computerName << userName;
    ui->twIps->addTopLevelItem(new QTreeWidgetItem(newItem, 0));
}

void PeerWindow::removeSyncablePeer(QString ip)
{
    for(unsigned int i = 0; i < ui->twIps->topLevelItemCount(); i++)
    {
        if(ui->twIps->topLevelItem(i)->text(0) == ip)
        {
            ui->twIps->takeTopLevelItem(i);
            break;
        }
    }
}

void PeerWindow::restore()
{
    ui->twIps->clear();
    ui->pbContinue->setEnabled(false);
}
