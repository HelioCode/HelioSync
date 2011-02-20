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
    emit addIp(ip);
}

void PeerWindow::on_twIps_clicked()
{
    ui->pbContinue->setEnabled(true);
}

void PeerWindow::displaySyncablePeer(QString ip, QString computerName, QString userName)
{
    QStringList newItem;
    newItem << ip << computerName << userName;
    ui->twIps->addTopLevelItem(new QTreeWidgetItem(newItem, 0));
}

void PeerWindow::displayProcessState(int process)
{
    ui->progressBar->setValue(process);
}

void PeerWindow::restore()
{
    ui->twIps->clear();
    ui->progressBar->setValue(0);
    ui->pbContinue->setEnabled(false);
}
