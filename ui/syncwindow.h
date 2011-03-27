#ifndef SYNCWINDOW_H
#define SYNCWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>

enum fileMode
{
    FMUpdate,
    FMNew,
    FMDelete
};

namespace Ui {
    class SyncWindow;
}

class SyncWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SyncWindow(QWidget *parent = 0);
    ~SyncWindow();

public slots:

private:
    Ui::SyncWindow *ui;
};

#endif // SYNCWINDOW_H
