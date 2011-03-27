#include "syncwindow.h"
#include "ui_syncwindow.h"

#include <QtDebug>

SyncWindow::SyncWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SyncWindow)
{
    ui->setupUi(this);
    QStringList itemList;
    itemList << "" << "Update" << "";
    QTreeWidgetItem* item = new QTreeWidgetItem(itemList);
    item->setCheckState(0, Qt::Checked);
    ui->twIncoming->addTopLevelItem(item);
    ui->twIncoming->resizeColumnToContents(0);
    ui->twIncoming->resizeColumnToContents(1);
    ui->twIncoming->itemAt(0, 0)->setCheckState(0, Qt::Unchecked);
    ui->twIncoming->clear();
    item = new QTreeWidgetItem(itemList);
    item->setCheckState(0, Qt::Checked);
    ui->twOutgoing->addTopLevelItem(item);
    ui->twOutgoing->resizeColumnToContents(0);
    ui->twOutgoing->resizeColumnToContents(1);
    ui->twOutgoing->itemAt(0, 0)->setCheckState(0, Qt::Unchecked);
    ui->twOutgoing->clear();
}

SyncWindow::~SyncWindow()
{
    delete ui;
}
